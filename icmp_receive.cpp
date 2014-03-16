/**
 *	Imie: Michal
 * 	Nazwisko: Lech
 *	nr indeksu: 248006
 */

#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <unistd.h> //pobieranie PID
#include <cstring>

#include "icmp_receive.h"
#include "sockwrap.h"
#include "icmp.h"
#include "timeval_op.h"

int				remaining_packet_data;
unsigned char* 	buffer_ptr;

void move_buffer_ptr(int count) {
    for (int i=0; i<count; i++) {
		buffer_ptr++; remaining_packet_data--;
	}
}

bool icmp_receive(int ttl, const int number_of_echo_request) {
    bool return_val = false;

    int number_of_echo_get = 0;
    int number_of_ip = 0;
    char* ip_adresses[3];

    printf("%d. ", ttl);

    unsigned char 	buffer[IP_MAXPACKET+1];

    // Tworzymy gniazdo surowe, od jadra bedziemy dostawac tylko pakiety ICMP.
    int sockfd = Socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);


    timeval t;      //czas oczekiwania na pakiety
    timeval t_0;    //poczatkowa wartosc czasu oczekiwania
    timeval t_temp; //czas czas ktory uplyna do odebrania pakietu
    double t_all = 0;   //suma czasow

    t.tv_sec = 1; //skundy
    t.tv_usec = 0;//mikrosekundy

    t_0.tv_sec = t.tv_sec;
    t_0.tv_usec = t.tv_usec;


    sockaddr_in sender;
    socklen_t sender_len = sizeof(sender);
	buffer_ptr = buffer;


    fd_set socks;
    FD_ZERO(&socks);
    FD_SET(sockfd, &socks);

    while (Select(sockfd + 1, &socks, NULL, NULL, &t) > 0) {
        remaining_packet_data = Recvfrom (sockfd, buffer_ptr, IP_MAXPACKET,
                                          0, &sender, &sender_len);

        //wyzaczenie czasu po jakim dotarl pakiet
        subtract_timeval(&t_0, &t, &t_temp);

        //pobranie ip z ktorej uzyskano pakiet
        char str_ip[20];
        inet_ntop(AF_INET, &(sender.sin_addr), str_ip, sizeof(str_ip));


        // Na poczatku bufora jest naglowek IP
        ip* packet = (ip*) buffer_ptr;

        move_buffer_ptr(packet->ip_hl * 4);

        icmp* icmp_packet = (icmp*) buffer_ptr;

        move_buffer_ptr(ICMP_HEADER_LEN);

        if (icmp_packet->icmp_type == ICMP_TIME_EXCEEDED &&
            icmp_packet->icmp_code == ICMP_EXC_TTL) {

            // Pakiet ICMP generowany w momencie zmniejszenia TTL do zera
            ip* packet_orig = (ip*) buffer_ptr;

            move_buffer_ptr(packet_orig->ip_hl * 4);

            if (packet_orig->ip_p == IPPROTO_ICMP) {
                // Ten pakiet zostal wygenerowany w odpowiedzi na pakiet IP:ICMP
                icmp* icmp_packet_org = (icmp*) buffer_ptr;

                //tutaj sprawdzanie pid i ttl(seq) czy sie zgadzaja
                if(icmp_packet_org->icmp_id == getpid() &&
                   icmp_packet_org->icmp_seq == ttl) {

                    switch(number_of_ip){
                        case 0:
                            ip_adresses[0] = str_ip;

                            printf("%s ", str_ip);
                            number_of_ip++;
                            break;

                        case 1:
                            if(strcmp(&ip_adresses[0][0], str_ip)!=0) {
                                ip_adresses[1] = str_ip;

                                printf("%s ", str_ip);
                                number_of_ip++;
                            }
                            break;

                        case 2:
                            if(strcmp(&ip_adresses[0][0], str_ip)!=0 &&
                               strcmp(&ip_adresses[1][0], str_ip)!=0) {

                                printf("%s ", str_ip);
                                number_of_ip++;
                            }
                            break;

                        default:
                            printf(" err ");

                            break;
                    }

                    number_of_echo_get++;
                    t_all += tv2ms(&t_temp);
                }
            }

        }

        if (icmp_packet->icmp_type == ICMP_ECHOREPLY) {

            move_buffer_ptr (remaining_packet_data);

            //tutaj sprawdzanie pid i ttl(seq) czy sie zgadzaja
            if(icmp_packet->icmp_id == getpid() &&
               icmp_packet->icmp_seq == ttl) {

                switch(number_of_ip){
                    case 0:
                        ip_adresses[0] = str_ip;
                        printf("%s ", str_ip);

                        number_of_ip++;
                        break;



                    case 1:
                        if(strcmp(&ip_adresses[0][0], str_ip) != 0) {
                            ip_adresses[1] = str_ip;

                            printf("%s ", str_ip);
                            number_of_ip++;
                        }
                        break;

                    case 2:
                        if(strcmp(&ip_adresses[0][0], str_ip) != 0 &&
                           strcmp(&ip_adresses[1][0], str_ip) != 0) {

                            printf("%s ", str_ip);
                            number_of_ip++;
                        }
                        break;

                    default:
                        printf("err ");
                        break;
                }

                number_of_echo_get++;
                t_all += tv2ms(&t_temp);

                return_val = true;
            }
        }
    }

    //ewentualne wypisanie czasu albo *
    if(number_of_echo_get == number_of_echo_request) {
        t_all /= number_of_echo_get;

        printf("%.2f ms\n", t_all);
    }else {
        if(number_of_echo_get == 0) {
            printf("*\n");
        }else {
            printf("???\n");
        }
    }

    return return_val;
}

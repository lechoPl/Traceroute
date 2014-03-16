/**
 *	Imie: Michal
 * 	Nazwisko: Lech
 *	nr indeksu: 248006
 */

#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "echo_request.h"
#include "icmp_receive.h"

using namespace std;

const int number_of_echo_request = 3;

bool check_IP(char* ip) {
   int number_of_coma=0;
   int temp;	

   for(int i=0; i<strlen(ip); i++){
	if(ip[i] == '.') {
	    temp = 0;
            number_of_coma++;
	    
	    if(number_of_coma > 3) return false;
	    
	    continue;   
        }
      	if(ip[i] < '0' || ip[i] > '9') return false;

	temp = temp*10 + (ip[i] - '0');
	
	if(temp > 255) return false;
   }
   return number_of_coma == 3;
};

int main(int argc, char** argv) {
    if (argc != 2) {
        printf ("Usage: sudo ./traceroute <ip>\n");
        exit(1);
    }

    if (!check_IP(argv[1])) {
        printf ("Wrong IP\n");
        exit(1);
    }



    for(int ttl=1; ttl<=30; ttl++) {
        for(int i=0; i<number_of_echo_request; i++) {
            echo_request(argv[1], ttl);
        }

        if(icmp_receive(ttl, number_of_echo_request)) break;
    }
}

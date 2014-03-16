/**
 *	Imie: Michal
 * 	Nazwisko: Lech
 *	nr indeksu: 248006
 */

#include <string>
#include <unistd.h> //pobieranie PID
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <cstring>
#include "echo_request.h"
#include "icmp.h"
#include "sockwrap.h"


void echo_request(std::string adress, int ttl){
    int sockfd = Socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

	sockaddr_in remote_address;
	memset(&remote_address, 0, sizeof(remote_address));

	remote_address.sin_family	= AF_INET;
	inet_pton(AF_INET, adress.c_str(), &remote_address.sin_addr);

	// Wysyłamy datagram do zdalnego serwera
	icmp icmp_packet;
	icmp_packet.icmp_type = ICMP_ECHO;
	icmp_packet.icmp_code = 0;
	icmp_packet.icmp_id = getpid();
	icmp_packet.icmp_seq = ttl;
	icmp_packet.icmp_cksum = 0;
	icmp_packet.icmp_cksum = in_cksum((u_short*)&icmp_packet, 8, 0);

	// Ustawiamy pole TTL pakietu na zadane
	Setsockopt (sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(int));

	Sendto(sockfd, &icmp_packet, ICMP_HEADER_LEN, 0, &remote_address, sizeof(remote_address));
};


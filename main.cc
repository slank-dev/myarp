
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "functions.h"


int main(int argc, char **argv){
	if(argc < 3){
		printf("usage: %s [ip address] [interface] \n", argv[0]);
		exit(-1);
	}

	u_int32_t ip = inet_addr(argv[1]);
	u_char targetmac[6];


	if(send_arp_request(ip, argv[2])<0)				exit(-1);
	if(recv_arp_reply(ip, argv[2], targetmac)<0)	exit(-1);



	for(int i=0; i<6; i++){
		printf("%2x", targetmac[i]);
		if(i<5)		printf(":");
		else		printf("\n");
	}


	return 0;
}

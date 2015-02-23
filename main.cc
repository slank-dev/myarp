
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "functions.h"

#include <net/ethernet.h>
#include <net/if_arp.h>
#include <netinet/if_ether.h>



int main(int argc, char **argv){
	if(argc < 3){
		printf("usage: %s [ip address] [interface] \n", argv[0]);
		exit(-1);
	}

	u_int32_t ip = inet_addr(argv[1]);
	u_char targetmac[6];

	if(send_arp_request(ip, argv[2])<0){
		printf("send_arp_request error\n");
		exit(-1);
	}

	if(recv_arp_reply(ip, argv[2], targetmac)<0){
		printf("recv_arp_reply error\n");
		exit(-1);
	}
	for(int i=0; i<6; i++){
		printf("%2x", targetmac[i]);
		if(i<5)		printf(":");
		else		printf("\n");
	}

	return 0;
}

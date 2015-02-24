
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "arp.h"
#include "addr.h"

#define MAX_DEVICES 1000



int main(int argc, char** argv){
	if(argc < 3){
		printf("usage: %s [interface] [device_count]\n", argv[0]);
		return -1;
	}


	u_int32_t alladdr[MAX_DEVICES];
	int addr_count = getaddrsinlan(argv[1], alladdr, MAX_DEVICES);
	u_char macaddr[6];
	char bender_name[256];
	int live_count;



	for(int i=0; i<atoi(argv[2]); i++){
		memset(macaddr, 0, sizeof(macaddr));

		send_arp_request(alladdr[i], argv[1]);
		if(recv_arp_reply(alladdr[i], argv[1], macaddr) != -999){
			live_count++;
			getbenderbymac(macaddr, bender_name);

			printf("%4d: %-16s\t", live_count, 
					addrtostr((unsigned int)alladdr[i]));
			for(int i=0; i<6; i++){
				printf("%02x", macaddr[i]);
				if(i<5)	printf(":");
				else	printf("\t");
			}
			printf("%s\n", bender_name);	
			continue;
		}

		memset(macaddr, 0, sizeof(macaddr));

		send_arp_request(alladdr[i], argv[1]);
		if(recv_arp_reply(alladdr[i], argv[1], macaddr) != -999){
			live_count++;
			getbenderbymac(macaddr, bender_name);

			printf("%4d: %-16s\t", live_count, 
					addrtostr((unsigned int)alladdr[i]));
			for(int i=0; i<6; i++){
				printf("%02x", macaddr[i]);
				if(i<5)	printf(":");
				else	printf("\t");
			}
			printf("%s\n", bender_name);	
			continue;
		}
	}
	

}



/*
 *
 *	Copyright (C) 2014-2015 Hiroki Shirokura <mail: slank.dev@gmail.com>
 *
 *	PROGRAMNAME is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  
 *	If not, see <http://slankdev.wordpress.com>.
 *	slank (Hiroki Shirokura) <mail: slank.dev@gmail.com>
 *
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "arp.h"
#include "addr.h"

#define MAX_DEVICES 1000

enum{PROGNAME, IFNAME};


int main(int argc, char** argv){
	if(argc < 2){
		printf("usage: %s [interface]\n", argv[PROGNAME]);
		return -1;
	}
	


	u_int32_t alladdr[MAX_DEVICES];
	u_char macaddr[6];
	int addr_count = getaddrsinlan(argv[IFNAME], alladdr, MAX_DEVICES);
	int live_count = 0;
	char bender_name[256];


	for(int i=0; i<addr_count; i++){
		print_ipaddr((unsigned int*)&alladdr[i]);
		send_arp_request(alladdr[i], argv[IFNAME]);
	}
	printf("%d devices can exist\n", addr_count);
	

}


/*
int main-old(int argc, char** argv){//[[[
	if(argc < 3){
		printf("usage: %s [interface] [device_count]\n", argv[0]);
		return -1;
	}


	u_int32_t alladdr[MAX_DEVICES];
	int addr_count = getaddrsinlan(argv[1], alladdr, MAX_DEVICES);
	u_char macaddr[6];
	char bender_name[256];
	int live_count;


	printf("Arp Scaning! \n");
	printf(" ---------------------------------------------------------------------\n");
	printf(" |    |   ip address          |    MAC address          |   bender   |\n");
	printf(" |-------------------------------------------------------------------|\n");
	for(int i=0; i<atoi(argv[2]); i++){
		memset(macaddr, 0, sizeof(macaddr));
		send_arp_request(alladdr[i], argv[1]);
		if(recv_arp_reply(alladdr[i], argv[1], macaddr) != -999){
			live_count++;
			getbenderbymac(macaddr, bender_name);

			printf(" | %2d |  %-15s      |  ", live_count, 
					addrtostr((unsigned int)alladdr[i]));
			for(int i=0; i<6; i++){
				printf("%02x", macaddr[i]);
				if(i<5)	printf(":");
				else	printf("      | ");
			}
			printf("%-10s |\n", bender_name);	
			continue;
		}
	}
	printf(" ---------------------------------------------------------------------\n");
	printf("%d devices are liveing\n", live_count);
	

}//]]]
*/


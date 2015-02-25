
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

#include "arp.h"
#include "addr.h"

#define MAX_DEVICES 1000
//#define DEBUG

#include <pcap.h>
#include <net/ethernet.h>
#include <netinet/if_ether.h>

int send_ArpRequest_AllAddr(const char* ifname){//[[[
	// wait just moment	
	
	u_int32_t alladdr[MAX_DEVICES];
	u_char macaddr[6];
	int addr_count = getaddrsinlan(ifname, alladdr, MAX_DEVICES);
	int live_count = 0;
	char bender_name[256];
	


	for(int i=0; i<addr_count; i++){
#ifdef DEBUG
		print_ipaddr((unsigned int*)&alladdr[i]);
#endif
		send_arp_request(alladdr[i], ifname);
	}
	printf("%d devices can exist\n", addr_count);

	

	return addr_count;
	// wait just moment
}//]]]


void func(u_char *nouse, const struct pcap_pkthdr *header,
										const u_char* packet){
	const u_char* packet0 = packet;
	struct ether_header* ethh;
	struct ether_arp *arp;

	ethh = (struct ether_header*)packet;
	packet += sizeof(struct ether_header);

	if(ntohs(ethh->ether_type) == ETHERTYPE_ARP){
		printf("arp!!!\n");
	}else{
		return;	
	}

}



int capture_main_loop(const char *ifname){
	char errbuf[PCAP_ERRBUF_SIZE];
	struct bpf_program fp;
	bpf_u_int32 mask;
	bpf_u_int32 net;
	pcap_t* handle;

	if(pcap_lookupnet(ifname, &net, &mask, errbuf) == -1){
		perror("pcap_lookupnet");
		return -1;
	}

	if((handle=pcap_open_live(ifname, 1000, 1, 1000, errbuf)) == NULL){
		perror("pcap_open_live");
		return -1;
	}

	pcap_loop(handle, 0, func, NULL);


	return 1;	
}


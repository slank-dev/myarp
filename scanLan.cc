
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

#include <pcap.h>
#include <net/ethernet.h>
#include <netinet/if_ether.h>

#include <unistd.h>
#include <sys/wait.h>

#include <netdb.h>

#define MAX_DEVICES 1000

int send_ArpRequest_AllAddr(const char* ifname){
	// wait just moment	
	usleep(800000);
	printf(" Send Arp to AlL in LAN \n");
	

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

#ifdef DEBUG_send_ArpReqest_AALAddr
	printf("%d devices can exist\n", addr_count);
#endif

	usleep(3000000);
	return addr_count;
}


void recvPackHandle(u_char *nouse, const struct pcap_pkthdr *header,
										const u_char* packet){
	const u_char* packet0 = packet;
	struct ether_header* ethh;
	struct ether_arp *arp;

	char mac_str[6];
	char bender_str[256];
	
	lc lc;
	struct hostent *host;


	ethh = (struct ether_header*)packet;
	packet += sizeof(struct ether_header);

	if(ntohs(ethh->ether_type) == ETHERTYPE_ARP){
		
		arp = (struct ether_arp*)packet;
		if(arp->arp_op == ntohs(ARPOP_REPLY)){
			printf("UP\t");
			
			for(int i=0; i<4; i++){
				printf("%d", arp->arp_spa[i]);
				lc.c[i] = arp->arp_spa[i];
				if(i<3)	fputc('.', stdout);
				else	fputc('\t', stdout);
			}

			for(int i=0; i<6; i++){
				printf("%x", arp->arp_sha[i]);
				if(i<5)	fputc(':', stdout);
				//else	fputc('\t', stdout);
			}

			getbenderbymac(arp->arp_sha, bender_str);
			host = gethostbyaddr((char*)lc.c, sizeof(lc), AF_INET);
			
			/*dns search need many time, so desplay is slow*/
			printf("(%s)\t", bender_str);
			if(host != NULL)	printf("%s\n", host->h_name);
			else				printf("\n");

			memset(bender_str, 0, sizeof(bender_str));
		}

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

	pcap_loop(handle, 0, recvPackHandle, NULL);


	return 1;	
}


int scanLan(const char* ifname){

	pid_t pid;
	int status;
	int addr_count;

	if((pid=fork()) == 0){
		
		addr_count = send_ArpRequest_AllAddr(ifname);


	}else{
		
		capture_main_loop(ifname);
		wait(&status);
		
	}

	return 1;
}

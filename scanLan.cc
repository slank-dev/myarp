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
#include <pcap.h>
#include <net/ethernet.h>
#include <netinet/if_ether.h>
#include <unistd.h>
#include <sys/wait.h>
#include <netdb.h>

#include <vector>

#include "arp.h"
#include "addr.h"
#include "slank.h"

#define MAX_DEVICES 1000


int send_ArpRequest_AllAddr(const char* ifname);
void recvPackHandle(u_char *nouse, 
		const struct pcap_pkthdr *header, const u_char* packet);
int scanLan(const char* ifname);




int send_ArpRequest_AllAddr(const char* ifname){
	u_int32_t alladdr[MAX_DEVICES];
	u_char macaddr[6];
	int addr_count = getaddrsinlan(ifname, alladdr, MAX_DEVICES);
	int live_count = 0;
	char bender_name[256];
	

	// wait just moment	
	usleep(800000);
	printf("[ArpSend in LAN Started] \n");

	for(int i=0; i<addr_count; i++){
		#ifdef DEBUG_send_ArpReqest_AALAddr
		print_ipaddr((unsigned int*)&alladdr[i]);
		#endif
		send_arp_request(alladdr[i], ifname);
	}

	usleep(5000000);
	printf("[Scan Finished]\n");
	return addr_count;
}


void recvPackHandle(u_char *data, const struct pcap_pkthdr *header,
										const u_char* packet){
	const u_char* packet0 = packet;
	struct ether_header* ethh;
	struct ether_arp *arp;
	struct hostent *host;
	struct device devbuf;
	char mac_str[6];
	char bender_str[256];
	//static std::vector<device> vec;
	lc lc;


	ethh = (struct ether_header*)packet;
	packet += sizeof(struct ether_header);

	if(ntohs(ethh->ether_type) == ETHERTYPE_ARP){
		arp = (struct ether_arp*)packet;
		if(arp->arp_op == ntohs(ARPOP_REPLY)){
			devbuf.live = true;
			
			for(int i=0; i<4; i++)	lc.c[i] = arp->arp_spa[i];
			for(int i=0; i<6; i++)	devbuf.ha[i] = arp->arp_sha[i];
			devbuf.pa = lc.l;

			getbenderbymac(arp->arp_sha, bender_str);
			host = gethostbyaddr((char*)lc.c, sizeof(lc), AF_INET);
			devbuf.bender = bender_str;

			/*dns search need many time, so desplay is slow*/
			if(host != NULL)	devbuf.hostname = host->h_name;
			
			devbuf.showinfo();
			devbuf.writeLog();

			memset(bender_str, 0, sizeof(bender_str));
		}
	}

}



int scanLan(const char* ifname){
	int addr_count;
	char errbuf[PCAP_ERRBUF_SIZE];
	struct bpf_program fp;
	bpf_u_int32 mask;
	bpf_u_int32 net;
	pcap_t* handle;
	pid_t pid;
	//std::vector<device> vec;


	if(pcap_lookupnet(ifname, &net, &mask, errbuf) == -1){
		perror("pcap_lookupnet");
		return -1;
	}
	if((handle=pcap_open_live(ifname, 0, 1, 1000, errbuf)) == NULL){
		perror("pcap_open_live");
		return -1;
	}
	


	if((pid=fork()) == 0){
		pcap_loop(handle, 0, recvPackHandle, NULL);
		pcap_close(handle);
	}else{
		addr_count = send_ArpRequest_AllAddr(ifname);
		kill(pid, SIGINT);
		wait(NULL);
	}



	return 1;
}

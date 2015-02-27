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





int send_ArpRequest_AllAddr(const char* ifname){//[[[
	u_int32_t alladdr[MAX_DEVICES];
	u_char macaddr[6];
	int addr_count = getaddrsinlan(ifname, alladdr, MAX_DEVICES);
	int live_count = 0;
	char bender_name[256];
	

	// wait just moment	
	usleep(800000);

	for(int i=0; i<addr_count; i++){
#ifdef DEBUG_send_ArpReqest_AllAddr
		print_ipaddr((unsigned int*)&alladdr[i]);
#endif
		send_arp_request(alladdr[i], ifname);
	}

	for(int i=0; i<addr_count; i++){
#ifdef DEBUG_send_ArpReqest_AllAddr
		print_ipaddr((unsigned int*)&alladdr[i]);
#endif
		send_arp_request(alladdr[i], ifname);
	}


	//usleep(5000000);
	usleep(8000000);
	return addr_count;
}//]]] 


 void recvPackHandle(u_char *data, const struct pcap_pkthdr *header,//[[[
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
			memset(bender_str, 0, sizeof(bender_str));
			
			devbuf.live = true;
			for(int i=0; i<4; i++)	lc.c[i] = arp->arp_spa[i];
			for(int i=0; i<6; i++)	devbuf.ha[i] = arp->arp_sha[i];
			devbuf.pa = lc.l;

			getbenderbymac(arp->arp_sha, bender_str);
			host = gethostbyaddr((char*)lc.c, sizeof(lc), AF_INET);
			devbuf.bender = bender_str;

			/*dns search need many time, so desplay is slow*/
			if(host != NULL)	devbuf.hostname = host->h_name;
			
			devbuf.getid();
			

			//devbuf.showinfo();
			devbuf.writeLog();
		}
	}

}//]]]



int scanLan(const char* ifname){
	int addr_count;
	char errbuf[PCAP_ERRBUF_SIZE];
	struct device dev;
	bpf_u_int32 mask;
	bpf_u_int32 net;
	pcap_t* handle;
	pid_t pid;
	std::vector<device> vec;

	FILE *fp;




	if(pcap_lookupnet(ifname, &net, &mask, errbuf) == -1){
		perror("pcap_lookupnet");
		return -1;
	}
	if((handle=pcap_open_live(ifname, 0, 1, 1000, errbuf)) == NULL){
		perror("pcap_open_live");
		return -1;
	}
	


	if((pid=fork()) == 0){
		printf("[ArpSend in LAN Started] \n");
		pcap_loop(handle, 0, recvPackHandle, NULL);
	}else{
		addr_count = send_ArpRequest_AllAddr(ifname);
		kill(pid, SIGINT);
		wait(NULL);
	}

	printf("[Scan Finished]\n");
	pcap_close(handle);
		
	printf("\n");

	//read log file
	if((fp=fopen(LOGFILE_NAME, "r")) == NULL){
		perror("scanLan fopen");
		return -1;
	}
		
	char str[256];
	unsigned int bid;
	char blive[16];
	char bipaddr[16];
	unsigned int bmac[6];
	char bbender[16];
	char bhostname[16];
	while(fgets(str, sizeof(str), fp) != NULL){
		memset(blive, 0, sizeof(blive));
		memset(bipaddr, 0, sizeof(bipaddr));
		memset(bmac, 0, sizeof(bmac));
		memset(bbender, 0, sizeof(bbender));
		memset(bhostname, 0, sizeof(bhostname));

		sscanf(str, "%u %s %s %x:%x:%x:%x:%x:%x %s %s", &bid, blive, bipaddr, 
				&bmac[0],&bmac[1],&bmac[2],&bmac[3],&bmac[4],&bmac[5],
				bbender, bhostname);

#ifdef DEBUG_scanLan
		printf("%s\t%s\t%2x:%2x:%2x:%2x:%2x:%2x\t%s\t%s \n", blive, bipaddr, 
				bmac[0],bmac[1],bmac[2],bmac[3],bmac[4],bmac[5],
				bbender, bhostname);
#endif


		if(strcmp("UP", blive) == 0)	dev.live=true;
		else							dev.live=false;
		for(int i=0; i<6; i++)	dev.ha[i] = bmac[i];
		dev.pa = inet_addr(bipaddr);
		dev.bender = bbender;
		dev.hostname = bhostname;


		//dev.showinfo();
		vec.push_back(dev);
	}

	printf("----------------------------------------------------------\n");
	for(int i=0; i<vec.size(); i++){
		vec[i].showinfo();	
	}
	printf("----------------------------------------------------------\n");



	return 1;
}

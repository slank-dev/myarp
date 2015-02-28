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
#include "scanLan.h"

#define MAX_DEVICES 1000





int send_ArpRequest_AllAddr(scanLanConfig sconfig){ 
	u_int32_t alladdr[MAX_DEVICES];
	u_char macaddr[6];
	int addr_count = getaddrsinlan(sconfig.ifname, alladdr, MAX_DEVICES);
	int live_count = 0;
	char bender_name[256];
	
	// wait just moment	
	usleep(0.8 * 1000000);
	

	for(int k=0; k<sconfig.scanLoopCount; k++){
		
		//printf("send count[%d]\n", k);
		for(int i=0; i<addr_count; i++){
#ifdef DEBUG_send_ArpReqest_AllAddr
			print_ipaddr((unsigned int*)&alladdr[i]);
#endif
			send_arp_request(alladdr[i], sconfig.ifname);
		}
		
		usleep(sconfig.timeout * 1000000);
	}

	//return addr_count;
	return 1;

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
			
			devbuf.writeLog();
		}
	}

}



int scanLan(scanLanConfig sconfig){
	int addr_count;
	char errbuf[PCAP_ERRBUF_SIZE];
	struct device dev;
	bpf_u_int32 mask;
	bpf_u_int32 net;
	pcap_t* handle;
	pid_t pid;
	std::vector<device> vec;

	FILE *fp;
	

	printf("--------------------------------\n");
	printf("ScaningInterface: %s(wlan0)\n", sconfig.ifname);
	printf("ScanCount: %d(1)\n", sconfig.scanLoopCount);
	printf("Timeout: %d(5) sec\n", sconfig.timeout);
	printf("--------------------------------\n");


	if(pcap_lookupnet(sconfig.ifname, &net, &mask, errbuf) == -1){
		perror("pcap_lookupnet");
		return -1;
	}
	if((handle=pcap_open_live(sconfig.ifname, 0, 0, 1000, errbuf)) == NULL){
		perror("pcap_open_live");
		return -1;
	}
	


	if((pid=fork()) == 0){
		printf("[ArpSend in LAN Started] \n");
		pcap_loop(handle, 0, recvPackHandle, NULL);
	}else{
		addr_count = send_ArpRequest_AllAddr(sconfig);
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
	unsigned int buf_id;
	char buf_live[16];
	char buf_ipaddr[16];
	unsigned int buf_mac[6];
	char buf_bender[16];
	char buf_hostname[16];
	while(fgets(str, sizeof(str), fp) != NULL){
		memset(buf_live, 0, sizeof(buf_live));
		memset(buf_ipaddr, 0, sizeof(buf_ipaddr));
		memset(buf_mac, 0, sizeof(buf_mac));
		memset(buf_bender, 0, sizeof(buf_bender));
		memset(buf_hostname, 0, sizeof(buf_hostname));

		sscanf(str, "%u %s %s %x:%x:%x:%x:%x:%x %s %s", 
				&buf_id, buf_live, buf_ipaddr, 
				&buf_mac[0],&buf_mac[1],&buf_mac[2],
				&buf_mac[3],&buf_mac[4],&buf_mac[5],
				buf_bender, buf_hostname);

#ifdef DEBUG_scanLan
		printf("%s\t%s\t%2x:%2x:%2x:%2x:%2x:%2x\t%s\t%s \n", 
				buf_live, buf_ipaddr, 
				buf_mac[0],buf_mac[1],buf_mac[2],
				buf_mac[3],buf_mac[4],buf_mac[5],
				buf_bender, buf_hostname);
#endif


		if(strcmp("UP", buf_live) == 0)	dev.live=true;
		else							dev.live=false;
		for(int i=0; i<6; i++)	dev.ha[i] = buf_mac[i];
		dev.pa = inet_addr(buf_ipaddr);
		dev.bender = buf_bender;
		dev.hostname = buf_hostname;


		vec.push_back(dev);
	}


	//show log info
	printf("----------------------------------------------------------\n");
	for(int i=0; i<vec.size(); i++){
		vec[i].showinfo();	
	}
	printf("----------------------------------------------------------\n");



	return 1;
}

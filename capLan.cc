
/*
 *Copyright (C) 2014-2015 Hiroki Shirokura <mail: slank.dev@gmail.com>
 *
 *TLexScan is free software: you can redistribute it and/or modify
 *it under the terms of the GNU General Public License as published by
 *the Free Software Foundation, either version 3 of the License, or
 *(at your option) any later version.
 *
 *TLexScan is distributed in the hope that it will be useful,
 *
 *but WITHOUT ANY WARRANTY; without even the implied warranty of
 *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *GNU General Public License for more details.
 *
 *You should have received a copy of the GNU General Public License
 *along with this program.  
 *If not, see <http://slankdev.wordpress.com>.
 *slank (Hiroki Shirokura) <mail: slank.dev@gmail.com>
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
#include <algorithm>
#include <time.h>

#include "arp.h"
#include "addr.h"
#include "myclass.h"
#include "util.h"
#include "debug.h"




void CaptureCallback(u_char *data, const struct pcap_pkthdr *header,
										const u_char* packet){
	const u_char* packet0 = packet;
	struct ether_header* ethh;
	struct ether_arp *arp;
	char mac_str[6];
	char bender_str[256];
	union lc{
		unsigned int l;
		unsigned char c[4];
	};
	lc lc;
	device devbuf;
	TLexOps* config = (TLexOps*)data;
	
	char timestr[256];
	time_t timer = time(NULL);
	struct tm *time;

	memset(timestr, 0, sizeof(timestr));
	time  = localtime(&timer);
	strftime(timestr, 255, "%H:%M:%S", time);



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
			devbuf.bender = bender_str;
			devbuf.lastchange = timestr;
			
			devbuf.getid();
			
			devbuf.writeLog(config->logname, 2);
		}
	}
} 





int CaptureLan(TLexOps opt){
	
	int space = 10;
	int addr_count;
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_t* handle;
	bpf_u_int32 mask;
	bpf_u_int32 net;
	const u_char* packet;
	struct pcap_pkthdr header;
	pid_t pid;




	if(pcap_lookupnet(opt.ifname, &net, &mask, errbuf) == -1){
		perror("pcap_lookupnet");
		return -1;
	}
	if((handle=pcap_open_live(opt.ifname, 0, 1, 1000, errbuf)) == NULL){
		perror("pcap_open_live");
		return -1;
	}

	if((pid=fork()) == 0){
		pcap_loop(handle, 0, CaptureCallback, (u_char*)&opt);
	}else{
		opt.scanLoopCount = 1;
		opt.timeout = 0;
		while(true){
			printf(" - Send arp packet to all addresses at %s\n", 
					gettimestr());
			
			addr_count = send_ArpRequest_AllAddr(opt);
			sleep(space);

			if(opt.verbose == 1){
				printf("\n");
				printLog(opt.logname);
				printf("\n");
			}
		}
		kill(pid, SIGINT);
		wait(NULL);
	}

	pcap_close(handle);
	return 1; 
}





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
#include <net/ethernet.h>
#include <net/if_arp.h>
#include <netinet/if_ether.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


#include "addr.h"
#include "debug.h"

#define deb  printf("debug!!(LINE:%d)\n", __LINE__)



int send_arp_request(const u_int32_t  ipaddr, const char* ifname){
	struct PACK_ARP{
		struct ether_header ethh;
		struct ether_arp 	arp;
	};
	union lc{
		unsigned long l;
		u_char	c[4];
	};
	
	
	int sock;
	struct sockaddr sa;
	struct PACK_ARP 	arp;
	union lc lc1, lc2;
	u_char 		*p;
	u_char		buf[sizeof(struct ether_header)+sizeof(struct ether_arp)];
	int 		total;
	const u_int32_t myip = inet_addr(get_paddr(ifname));
	const u_char mac_bcast[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
	u_char mymac[6];
	get_haddr(ifname, mymac);		// UseMyfunction


	if((sock=socket(PF_PACKET, SOCK_PACKET, htons(ETH_P_ARP)))<0){
		perror("send_socket");
		return -1;
	}

	memset(&sa, 0, sizeof(sa));
	sa.sa_family = PF_PACKET;
	snprintf(sa.sa_data, sizeof(sa.sa_data), "%s", ifname);

	if(bind(sock, &sa, sizeof(sa)) < 0){
		perror("send_bind");
		return -1;
	}

	//config arp_hdr
	arp.arp.arp_hrd=htons(ARPHRD_ETHER);
	arp.arp.arp_pro=htons(ETHERTYPE_IP);
	arp.arp.arp_hln=6;
	arp.arp.arp_pln=4;
	arp.arp.arp_op=htons(ARPOP_REQUEST);
	
	//config arp(mac)
	for(int i=0; i<6; i++){
		arp.arp.arp_sha[i]=mymac[i];
		arp.arp.arp_tha[i]=0;
	}

	//config arp(ip)
	lc1.l=myip;
	lc2.l=ipaddr;
	for(int i=0; i<4; i++){
		arp.arp.arp_spa[i]=lc1.c[i];
		arp.arp.arp_tpa[i]=lc2.c[i];
	}
	
	//config eth(mac) only
	for(int i=0; i<6; i++){
		arp.ethh.ether_dhost[i]=mac_bcast[i];
		arp.ethh.ether_shost[i]=mymac[i];
	}
	arp.ethh.ether_type = htons(ETHERTYPE_ARP);
	
	//make final packet 
	memset(buf, 0, sizeof(buf));
	p = buf;
	memcpy(p,&arp.ethh,sizeof(struct ether_header));
	p += sizeof(struct ether_header);
	memcpy(p, &(arp.arp), sizeof(struct ether_arp));
	p += sizeof(struct ether_arp);
	total = p-buf;


	//send packet
	if((sendto(sock, buf, total, 0, &sa, sizeof(sa)))<0){
		perror("send_sendto");
		return -1;
	}
#ifdef DEBUG_send_arp_request
	printf("[DEBUG] in function \"%s\" %s:%d  ", 
					__func__, __FILE__, __LINE__);
	print_ipaddr((unsigned int*)&ipaddr);	// UseMyfunction
#endif 

	close(sock);

	return 1;
}




int recv_arp_reply(const u_int32_t ipaddr, 
				const char* ifname, u_char macaddr[6]){

	union lc{
		unsigned long l;
		u_char c[4];
	};

	int sock;
	struct sockaddr sa;
	int len_sa = sizeof(struct sockaddr);
	struct ether_header* ethp;
	struct ether_arp* 	arpp;
	lc lc1;
	u_char buf[sizeof(struct ether_header)+sizeof(struct ether_arp)];
	u_char *p;
	int total;
	const u_int32_t myip = inet_addr(get_paddr(ifname));	//UseMyfunction
	u_char mymac[6];
	get_haddr(ifname, mymac);			// UseMyfunction

	fd_set readfds;
	fd_set fds;
	struct timeval timeout;
	
	
	// if arp to mine
	if(ipaddr == myip){
		get_haddr(ifname, macaddr);		// UseMyfunction
		return 1;
	}




	// endif



	if((sock=socket(PF_PACKET, SOCK_PACKET, htons(ETH_P_ARP)))<0){
		perror("recv_socket");
		return -1;
	}

	memset(&sa, 0, sizeof(sa));
	sa.sa_family = PF_PACKET;
	snprintf(sa.sa_data, sizeof(sa.sa_data), "%s", ifname);
	
	if(bind(sock, &sa, sizeof(sa)) < 0){
		perror("recv_bind");
		close(sock);
		return -1;
	}

	timeout.tv_sec = 0;			// 1 sec
	timeout.tv_usec = 500000;	// 500000 micro sec
	FD_ZERO(&readfds);
	FD_SET(sock, &readfds);	
	
	while(1){
		p = buf;
		memset(buf, 0, sizeof(buf));		
		memcpy(&fds, &readfds, sizeof(fd_set));

		if(select(sock+1, &fds, NULL, NULL, &timeout)==0){
			//fprintf(stderr, "select timeout: target may be down\n");
			close(sock);
			return -999;
		}
		
		if(FD_ISSET(sock, &fds)){
			total = recvfrom(sock, buf, sizeof(buf), 0, &sa, (unsigned int*)&len_sa);
			
			//open packet to ethp and arpp
			ethp = (struct ether_header*)p;
			p += sizeof(struct ether_header);
			arpp = (struct ether_arp*)p;
			
			//check packet's header
			if(ethp->ether_type==htons(ETHERTYPE_ARP) &&
					arpp->arp_op==htons(ARPOP_REPLY) ){
				for(int i=0; i<6; i++){
					if(arpp->arp_tha[i] != mymac[i]){
						printf("recved arp packet is not for me\n");
						continue;	
					}
				}				
				for(int i=0; i<6; i++)
					macaddr[i] = arpp->arp_sha[i];

				return 1;
			}
		}
	}
}



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
#include <unistd.h> 	//for close()
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>

#include <stdlib.h>
#include <stdio.h>

#include "addr.h"
#include "arp.h"
#include "slank.h"



 

char* get_paddr(const char* ifname){
	int sockd;
	struct ifreq ifr;
	struct sockaddr_in *sa;
	char* ipstr;

	if ((sockd=socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("socket()!");
		exit(-1);
	}

	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ-1);
	ioctl(sockd, SIOCGIFADDR, &ifr);
	close(sockd);
	
	sa = (struct sockaddr_in*)&ifr.ifr_addr;

	ipstr = inet_ntoa(sa->sin_addr);
	return ipstr;
}

char* get_pmask(const char* ifname){
	int sockd;
	struct ifreq ifr;
	struct sockaddr_in *sa;
	char* maskstr;

	if((sockd=socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("socket");
		exit(-1);
	}

	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ-1);
	ioctl(sockd, SIOCGIFNETMASK, &ifr);
	close(sockd);

	sa = (struct sockaddr_in*)&ifr.ifr_addr;
	maskstr = inet_ntoa(sa->sin_addr);

	return maskstr;
}



void get_haddr( const char* ifname, u_char haddr[6]){
	int sockd;
	struct ifreq ifr;

	if ((sockd=socket(AF_INET,SOCK_DGRAM,0)) < 0){
		perror("socket()!");
		exit(-1);
	}

	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ-1);
	
	ioctl(sockd, SIOCGIFHWADDR, &ifr);
	close(sockd);
	
	for(int i=0; i<6; i++)	
		haddr[i] = (unsigned char)ifr.ifr_hwaddr.sa_data[i];

}



void count_next_addr(unsigned int *p){
	union lc *lc = (union lc*)p;
	
	for(int i=3; i>=0; i--){
		if(lc->c[i] >= 255){
			lc->c[i]=0;
			lc->c[i-1];
		}else{
			lc->c[i]++;
			return;
		}
	}
}



void print_ipaddr( unsigned int* addr){
	union lc *lc = (union lc*)addr;
	for(int i=0; i<4; i++){
		printf("%d", lc->c[i]);
		if(i<3)		printf(".");
		else		printf("\n");
	}
}



char* addrtostr(const unsigned int addr){
	char *str;
	union lc lc;
	lc.l = (int)addr;
	str = (char*)malloc(sizeof(char)*16);
	snprintf(str,sizeof(char[16]),"%u.%u.%u.%u",
					lc.c[0],lc.c[1],lc.c[2],lc.c[3]);
	return str;
}



int getclassbyaddr(unsigned int addr){
	union lc lc;
	lc.l = addr;

	if(0 <= lc.c[0] && lc.c[0] <= 127)		return 0;	// class A
	else if(128 <= lc.c[0] && lc.c[0] <= 191)	return 1;	// class B
	else if(192 <= lc.c[0] && lc.c[0] <= 233)	return 2;	// class C
	else 	return -1;	// error
}



int getaddrsinlan(const char* ifname,  u_int32_t alladdr[], int size){
	int  addr_count;
	char bender_name[256];
	const u_int32_t myip = inet_addr(get_paddr(ifname));
	const u_int32_t mask = inet_addr(get_pmask(ifname));
	union lc lc;
	u_int32_t startaddr = (mask & myip);
	u_char macaddr[6];
	
	lc.l = startaddr;
	for(int i=0; i<4; i++){
		if(lc.c[i]==0)	lc.c[i]=255;	
	}

	count_next_addr((unsigned int*)&startaddr);
	const u_int32_t endaddr = (u_int32_t)lc.l;
	u_int32_t addr = startaddr;
	
	#ifdef DEBUG_getaddrsinlan
	printf("\n[DEBUG] in function \"%s\"  %s:%d\n", __func__, __FILE__, __LINE__);
	printf("---scan-info------------------------\n");
	printf("your ip : %s\n", addrtostr((unsigned int)myip));
	printf("netmask : %s\n", addrtostr((unsigned int)mask));
	printf("start   : %s\n", addrtostr((unsigned int)startaddr));
	printf("end(max): %s\n", addrtostr((unsigned int)endaddr));
	printf("------------------------------------\n\n");
	#endif
	
	for(addr_count=0; addr != endaddr && addr_count<size; addr_count++){
		alladdr[addr_count] = addr;
		count_next_addr((unsigned int*)&addr);
	}
	return addr_count;
}

 

void getbenderbymac(const u_char data[6], char* bender){
	FILE *fp;
	const char* filename = "mac_code.txt";
	char strbuf[256];
	unsigned int  mac[3];
	u_char dev_mac[3] = {data[0],data[1],data[2]};
	

	if((fp=fopen(filename, "r")) == NULL){
		perror("getbenderbymac fopen");
		strcpy(bender, "getbenderbymac error");
		return;
	}

	while(fgets(strbuf, sizeof(strbuf), fp) != NULL){
		sscanf(strbuf, "%2x%2x%2x\t%s", &mac[0],&mac[1],&mac[2],bender);
		
		if(mac[0]==dev_mac[0]&&mac[1]==dev_mac[1]&&mac[2]==dev_mac[2]){
			#ifdef DEBUG_getbenderbymac
			printf("\n[DEBUG] in function \"%s\" %s:%d  \n", __func__, __FILE__, __LINE__);
			printf("search hit!  %02X:%02x:%02x (%s) \n\n",mac[0],mac[1],mac[2],bender);
			#endif
			return;
		}

		memset(mac, 0, sizeof(mac));
		memset(bender, 0, sizeof(bender));
		memset(strbuf, 0, sizeof(strbuf));
	}
	
	strcpy(bender, "not-found");

	fclose(fp);
	
	return;
}

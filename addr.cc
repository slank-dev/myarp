// addr.cc


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



char* get_paddr(const char* ifname);
void  get_haddr( const char* ifname, u_char haddr[6]);

void count_next_addr(unsigned int *p);
void print_ipaddr(unsigned int* addr);
char* addrtostr(const unsigned int addr);
int getclassbyaddr(unsigned int addr);
int getaddrsinlan(const char *ifname, int size);





// Function Name	get_paddr(ifname)
// Description		get ip address from ifname
// Return value		char* ipaddr_string
// Arguments		ifname->  interface name
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



// Function Name	get_haddr(ifname)
// Description		get MAC address from ifname
// Return Value		u_char* (u_char[6]) address
// Arguments		ifname->	interface name
void get_haddr( const char* ifname, u_char haddr[6]){
	int sockd;
	struct ifreq ifr;	//<net/if.h>

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



int getaddrsinlan(const char* ifname, int size){
	u_int32_t myip = inet_addr(get_paddr(ifname));
	
	int addr_class_id = getclassbyaddr((unsigned int)myip);
	if(addr_class_id < 0){
		fprintf(stderr, "getclassbyaddr: address error \n");
		return -1;
	}

	const u_int32_t mask[3] = {
		inet_addr(IPMASK_CLASS_A),
		inet_addr(IPMASK_CLASS_B),
		inet_addr(IPMASK_CLASS_C)	
	};

	const u_int32_t startaddr = (mask[addr_class_id] & myip);
	union lc lc;
	lc.l = startaddr;
	
	for(int i=0; i<4; i++){
		if(lc.c[i]==0)	lc.c[i]=255;	
	}
	count_next_addr((unsigned int*)&startaddr);
	
	const u_int32_t endaddr = (u_int32_t)lc.l;
	u_int32_t addr = startaddr;
	
	printf("---scan-info------------------------\n");
	printf("your ip : %s\n", addrtostr((unsigned int)myip));
	printf("netmask : %s\n", addrtostr((unsigned int)mask[2]));
	printf("start   : %s\n", addrtostr((unsigned int)startaddr));
	printf("end(max): %s\n", addrtostr((unsigned int)endaddr));
	printf("------------------------------------\n\n");
	
	
	u_char macaddr[6];
	int count=0;
	if(size == 0)	size = 10000000;
	for(int i=0; addr != endaddr && i<size; i++){
		memset(macaddr, 0, sizeof(macaddr));
		
		send_arp_request(addr, ifname);

		if(recv_arp_reply(addr, ifname, macaddr) != -999){
			printf("%4d: %-16s    ", count+1, addrtostr((unsigned int)addr));
			
			for(int i=0; i<6; i++){
				printf("%02x", macaddr[i]);
				if(i<5)	printf(":");
				else	printf("\n");
			}
			count++;
		}
					
			


		count_next_addr((unsigned int*)&addr);
	}
	return count;
}


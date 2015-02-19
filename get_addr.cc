
#include "functions.h"

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

 
// Function Name	get_paddr(ifname)
// Description		get ip address from ifname
// Return value		char* ipaddr_string
// Arguments		ifname->  interface name
char* get_paddr(const char* ifname){
	int sockd;
	struct ifreq ifr;
	struct sockaddr_in *sa;
	char* ipstr;

	if((sockd=socket(AF_INET, SOCK_DGRAM, 0)) < 0){
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
u_char* get_haddr( const char* ifname){
	static u_char *haddr;
	int sockd;
	struct ifreq ifr;	//<net/if.h>

	haddr = (u_char*)malloc(sizeof(u_char)*6);

	if((sockd=socket(AF_INET,SOCK_DGRAM,0)) < 0){
		perror("socket()!");
		exit(-1);
	}

	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ-1);
	
	ioctl(sockd, SIOCGIFHWADDR, &ifr);
	close(sockd);
	
	for(int i=0; i<6; i++)	haddr[i] = (unsigned char)ifr.ifr_hwaddr.sa_data[i];

	return haddr;
}






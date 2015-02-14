#include <stdio.h>



#include <unistd.h>		//for close()
#include <string.h>
#include <stdlib.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <netinet/in.h>

void get_macaddr(u_char* haddr, const char* ifname){
	
	int sockd;
	struct ifreq ifr;	//<net/if.h>

	if((sockd=socket(AF_INET,SOCK_DGRAM,0)) < 0){
		perror("socket()!");
		exit(-1);
	}

	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ-1);
	
	ioctl(sockd, SIOCGIFHWADDR, &ifr);
	close(sockd);
	
	for(int i=0; i<6; i++)	haddr[i] = (unsigned char)ifr.ifr_hwaddr.sa_data[i];
}


int main(){
	u_char strs[6];

	get_macaddr(strs, "wlan0");

	for(int i=0; i<6; i++)	printf("%x:", strs[i]);
	printf("\n");

	return 0;
}


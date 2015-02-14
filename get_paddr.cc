#include <stdio.h>



#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>

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

int main(){
	char* str = get_paddr("wlan0");
	printf("%s\n", str);
}

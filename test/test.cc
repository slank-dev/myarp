
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <net/ethernet.h>
#include <net/if_arp.h>
#include <netinet/if_ether.h>
#include <unistd.h>	//for write()

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>

#define deb  printf("debug!!(LINE:%d)\n", __LINE__)


typedef struct{
	struct ether_header ethh;
	struct ether_arp 	arp;
}PACK_ARP;

void send_arp_request(const u_int32_t  ipaddr, const char* ifname){
	int sock;
	struct sockaddr sa;

	PACK_ARP 	arp;
	u_char 		*p;
	u_char		buf[sizeof(struct ether_header)+sizeof(struct ether_arp)];
	int 		total;
	
	union lc{
		unsigned long l;
		u_char	c[4];
	};
	lc lc1, lc2;
	
	
	int len = sizeof(struct ether_header) + sizeof(struct ether_arp);
	const u_int32_t myip = inet_addr("192.168.179.8");
	const u_char mymac[6] = {0xcc,0xe1,0xd5,0x15,0x17,0xb5};
	const u_char mac_bcast[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
	

	
	if((sock=socket(PF_PACKET, SOCK_PACKET, htons(ETH_P_ARP)))<0){
		perror("socket()!!");
		exit(-1);
	}

	memset(&sa, 0, sizeof(sa));
	sa.sa_family = PF_PACKET;
	snprintf(sa.sa_data, sizeof(sa.sa_data), "%s", ifname);

	if(bind(sock, &sa, sizeof(sa)) < 0){
		perror("bind()!!");
		exit(-1);
	}
	
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

	
	//print info
	printf("TargetProtcolAddr: ");
	for(int i=0; i<4; i++){
		printf("%d", arp.arp.arp_tpa[i]);
		if(i!=3)	printf(".");
		else		printf("\n");
	}
	
	memset(buf, 0, sizeof(buf));
	p = buf;
	 memcpy(p,&arp.ethh,sizeof(struct ether_header));
	p += sizeof(struct ether_header);
	memcpy(p, &(arp.arp), sizeof(struct ether_arp));
	p += sizeof(struct ether_arp);
	total = p-buf;

	while(1){
		if((sendto(sock, buf, total, 0, &sa, sizeof(sa)))<0){
			perror("write()!!");
			exit(-1);
		}
		printf("  - arp sent to target\n");
	}
}


int main(){
	u_int32_t ip = inet_addr("192.168.179.4");
	send_arp_request(ip, "wlan0");
	
	deb;
	return 0;
}

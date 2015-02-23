
//for get_paddr(), get_haddr()
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



char* get_paddr(const char* ifname);
void  get_haddr( const char* ifname, u_char haddr[6]);


int send_arp_request(const u_int32_t  ipaddr, const char* ifname);
int recv_arp_reply(const u_int32_t ipaddr, 
				const char* ifname, u_char macaddr[6]);

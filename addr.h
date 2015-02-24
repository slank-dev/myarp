
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

#define IPMASK_CLASS_A "255.0.0.0"
#define IPMASK_CLASS_B "255.255.0.0"
#define IPMASK_CLASS_C "255.255.255.0"



union lc{
	unsigned int l;
	unsigned char c[4];
};



char* get_paddr(const char* ifname);
void  get_haddr( const char* ifname, u_char haddr[6]);

void count_next_addr(unsigned int *p);
void print_ipaddr(unsigned int* addr);
char* addrtostr(const unsigned int addr);
int getclassbyaddr(unsigned int addr);
int getaddrsinlan(const char *ifname, int size);


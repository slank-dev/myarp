#include <arpa/inet.h>		//for main()
#include <netinet/in.h>		//for main()


#include <stdlib.h>
#include <stdio.h>
#include "ipaddr.h"
#include "get_addr.h"


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

int getaddrsinlan(const char* ifname, char** addrs, int size){
	addrs = (char**)malloc(sizeof(char[16]) * sizeof(char[size]));
	memset(addrs, '\0', sizeof(char[16])* sizeof(char[size]));
	int count;
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
	
	/*
	printf("ipaddr : %s\n", addrtostr((unsigned int)myip));
	printf("netmask: %s\n", addrtostr((unsigned int)mask[2]));
	printf("start  : %s\n", addrtostr((unsigned int)startaddr));
	printf("end    : %s\n", addrtostr((unsigned int)endaddr));
	*/
	char* buf;
	for(count=0; addr != endaddr && count<size; count++){
	//	printf("%4d: ", count);
	//	print_ipaddr((unsigned int*)&addr);
		count_next_addr((unsigned int*)&addr);
		buf = addrtostr((unsigned int)addr);
		strcpy(*(addrs+count), buf);
	}
	return count;

}



int main(){
	char** strs;
	getaddrsinlan("wlan0", strs, 256);
	

}

#include <stdio.h>

#define IPMASK_CLASS_A "255.0.0.0"
#define IPMASK_CLASS_B "255.255.0.0"
#define IPMASK_CLASS_C "255.255.255.0"

union lc{
	unsigned int l;
	unsigned char c[4];
};

void count_next_addr(int *p);
void print_ipaddr(unsigned int* addr);

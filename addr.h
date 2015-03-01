
/*
 *Copyright (C) 2014-2015 Hiroki Shirokura <mail: slank.dev@gmail.com>
 *
 *TLexScan is free software: you can redistribute it and/or modify
 *it under the terms of the GNU General Public License as published by
 *the Free Software Foundation, either version 3 of the License, or
 *(at your option) any later version.
 *
 *TLexScan is distributed in the hope that it will be useful,
 *
 *but WITHOUT ANY WARRANTY; without even the implied warranty of
 *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *GNU General Public License for more details.
 *
 *You should have received a copy of the GNU General Public License
 *along with this program.  
 *If not, see <http://slankdev.wordpress.com>.
 *slank (Hiroki Shirokura) <mail: slank.dev@gmail.com>
 *
 */

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
char* get_pmask(const char* ifname);
void  get_haddr( const char* ifname, u_char haddr[6]);

void count_next_addr(unsigned int *p);
void print_ipaddr(unsigned int* addr);
char* addrtostr(const unsigned int addr);
int getclassbyaddr(unsigned int addr);
int getaddrsinlan(const char *ifname, u_int32_t alladdr[], int size);

void getbenderbymac(const u_char data[6], char* bender);

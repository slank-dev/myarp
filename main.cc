/*
 *
 *	Copyright (C) 2014-2015 Hiroki Shirokura <mail: slank.dev@gmail.com>
 *
 *	PROGRAMNAME is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  
 *	If not, see <http://slankdev.wordpress.com>.
 *	slank (Hiroki Shirokura) <mail: slank.dev@gmail.com>
 *
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>

#include <unistd.h>

#include "arp.h"
#include "addr.h"
#include "scanLan.h"
#include "slank.h"
#include "util.h"

#include <iostream>


enum{PROGNAME, IFNAME};




int main(int argc, char** argv){
	int opt;
	char ifname[32];
	int count;
	int timeout;

	strncpy(ifname, "wlan0", sizeof("wlan0"));	//set default interface
	count = 1;
	timeout = 5;
		
	/*read options*/
	while((opt=getopt(argc, argv, "hvi:c:t:")) != -1){
		switch(opt){
			case 'h':
				printf("[opt]help\n");
				break;
			case 'v':
				printf("[opt]version\n");
				break;
			case 'i':
				strncpy(ifname, optarg, sizeof(ifname));
				printf("[opt select interface] iface: %s\n", ifname);
				break;
			case 'c':
				count = atoi(optarg);
				printf("[opt set count] count: %d \n", atoi(optarg));
				break;
			case 't':
				timeout = atoi(optarg);
				printf("[opt set timeout] timeout: %d \n", atoi(optarg));
				break;
		}
	}
	
		

	
	printf("\n");
	scanLan(ifname, count, timeout);
	printf("\n");

}




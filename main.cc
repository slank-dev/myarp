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
	int c;
	char ifname[32];

	strncpy(ifname, "wlan0", sizeof("wlan0"));	//set default interface

	/*read options*/
	while((c=getopt(argc, argv, "hvi:")) != -1){
		switch(c){
			case 'h':
				printf("[opt]help\n");
				break;
			case 'v':
				printf("[opt]version\n");
				break;
			case 'i':
				printf("[opt]select interface\n");
				strncpy(ifname, optarg, sizeof(ifname));
				printf("iface: %s\n", ifname);
		}
	}
	
		

	
	printf("\n");
	scanLan(ifname);
	printf("\n");

}




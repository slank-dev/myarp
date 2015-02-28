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






int main(int argc, char** argv){
	int opt;
	char ifname[32];
	int count;
	int timeout;
	scanLanConfig conf;

	

	/*read options*/
	while((opt=getopt(argc, argv, "hvi:c:t:f:")) != -1){
		switch(opt){
			case 'h':
				usage(argc, argv);
				return 1;
				break;
			case 'v':
				version();
				return 1;
				break;
			case 'f':
				printf("load file only()() korekara!!\n");
				return 1;
				break;
			case 'i':
				strncpy(conf.ifname, optarg, sizeof(conf.ifname));
				break;
			case 'c':
				conf.scanLoopCount = atoi(optarg);
				break;
			case 't':
				conf.timeout = atoi(optarg);
				break;
		}
	}
	
		
	
	printf("\n");
	scanLan(conf);
	printf("\n");

}




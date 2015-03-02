
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
#include <vector>
#include <unistd.h>

#include <sys/time.h>

#include "scanLan.h"	// for class scanLanConfig
#include "util.h"
#include "myclass.h"
#include "debug.h"




int main(int argc, char** argv){
	int opt;
	struct timeval s, e;
	clock_t startTime, endTime;
	scanLanConfig conf;		// UseMyDataType

	gettimeofday(&s, NULL);
	
	
	while((opt=getopt(argc, argv, "nhvi:c:t:p:s:f:")) != -1){
		switch(opt){
			case 'n':
				conf.verbose = 0;
				break;
			case 'h':
				usage(argc, argv);
				return 1;
				break;
			case 'v':
				version();
				return 1;
				break;
			case 'p':
				sortLog(optarg);		
				printLog(optarg);
				return 1;
				break;
			case 's':
				sortLog(optarg);
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
			case 'f':
				strcpy(conf.logname, optarg);
				break;
		}
	}
	
	printf("\n");
	printf("Starting TLex 0.10 \n");
	conf.showConfig();
	//printf("\n");

	scanLan(conf);
	gettimeofday(&e, NULL);

	startTime = s.tv_sec + (double)(s.tv_usec * 1e-6);
	endTime = e.tv_sec + (double)(e.tv_usec * 1e-6);

	printf("\n");
	printf("TLex done: scan finished in ");
	printf("%lu.%lu sec\n",
			(long)e.tv_sec-s.tv_sec, (long)e.tv_usec-s.tv_usec);

}




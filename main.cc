
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
#include <getopt.h>
#include <pcap.h>

#include "scanLan.h"	// for class TLexOps
#include "util.h"
#include "myclass.h"
#include "debug.h"
#include "lscanLan.h"
#include "monLan.h"




int main(int argc, char** argv){
	TLexOps opt;
	TLexInfo info;
	mytimer tm;
	

	parse_option(argc, argv, opt);
	

	
	switch(opt.mode){
		case TLEXMODE_HELP:
			usage(argc,argv);
			break;
		case TLEXMODE_VERSION:
			version();
			break;

		case TLEXMODE_PRINTLOG:
			printLog(opt.logname);
			break;
		
		case TLEXMODE_SORTLOG:
			sortLog(opt.logname);
			break;
		
		case TLEXMODE_SCAN_NORMAL:
			printf("\nStarting %s %s\n", info.str(), gettimestr());
			opt.showConfig();

			tm.mytimer_start();
			ScanLan(opt);
			tm.mytimer_end();

			printf("\nTLex done: scan finished in %s sec \n",tm.intervalStr());
			break;
		
		case TLEXMODE_SCAN_MONITOR:
			printf("\nStarting %s %s\n", info.str(), gettimestr());
			opt.showConfig();

			tm.mytimer_start();
			MonitorLan(opt);	
			tm.mytimer_end();

			printf("\nTLex done: scan finished in %s sec \n",tm.intervalStr());
			break;

		case TLEXMODE_SCAN_LONG:
			printf("\nStarting %s %s\n", info.str(), gettimestr());
			
			printf("--------------------------------\n");
			printf("Interface    :  %-10s     \n", opt.ifname);
			printf("logfile      :  %-10s     \n", opt.logname);
			printf("verbose      :  ");
			if(opt.verbose == 0)		printf("OFF\n");
			else if(opt.verbose == 1)	printf("ON\n");
			else if(opt.verbose == 2)	printf("SEMI\n");
			printf("--------------------------------\n");

			tm.mytimer_start();
			LongScanLan(opt);
			tm.mytimer_end();

			printf("\nTLex done: scan finished in %s sec \n",tm.intervalStr());
			break;
	}

	return 1;



}




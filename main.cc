
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

#include "scanLan.h"	// for class TLexOps
#include "util.h"
#include "myclass.h"
#include "debug.h"




void MonitorLan(TLexOps opt){
	printf("monitoring lan!!\n");
	return;
}


void CaptureLan(TLexOps opt){
	printf("capturing lan!!\n");
	return;
}





int main(int argc, char** argv){
	struct timeval s, e;
	clock_t startTime, endTime;
	TLexOps conf;
	TLexInfo info;
	

	gettimeofday(&s, NULL);


	parse_option(argc, argv, conf);
	if(conf.mainopt[TLEXOPT_HELP]){
		usage(argc,argv);
		return 1;
	}
	else if(conf.mainopt[TLEXOPT_VERSION]){
		version();
		return 1;	
	}
	else if(conf.mainopt[TLEXOPT_PRINTLOG]){
		printLog(conf.logname);
		return 1;	
	}
	else if(conf.mainopt[TLEXOPT_SORTLOG]){
		sortLog(conf.logname);
		return 1;	
	}

	


	if(conf.mode == 1){
		printf("\nStarting %s \n", info.str());
		conf.showConfig();

		scanLan(conf);
		gettimeofday(&e, NULL);

		startTime = s.tv_sec + (double)(s.tv_usec * 1e-6);
		endTime = e.tv_sec + (double)(e.tv_usec * 1e-6);

		printf("\nTLex done: scan finished in %lu.%lu sec \n",
				(long)e.tv_sec-s.tv_sec, (long)e.tv_usec-s.tv_usec);
	
	}else if(conf.mode == 2){
		MonitorLan(conf);	
	}else if(conf.mode == 3){
		CaptureLan(conf);	
	}


}




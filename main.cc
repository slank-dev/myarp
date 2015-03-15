
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





void parse_option(int argc, char** argv, TLexOps& conf){
	int opt;
	struct option long_options[] = {
		{"version", no_argument, 0, 0},
		{"help", no_argument, 0, 0},
		{"monitor", no_argument, 0, 0},
		{"interface", required_argument, 0, 0},
		{"timeout", required_argument, 0, 0},
		{"loop", required_argument, 0, 0},
		{"file", required_argument, 0, 0}
	};
	int opt_index;


	while((opt=getopt_long(argc,argv, "hvi:l:t:p:s:f:", long_options, &opt_index)) != -1){
		switch(opt){
			case 0:
				/* dont get argument */
				if(strcmp(long_options[opt_index].name, "version") == 0){
					printf("--version version()\n");
					conf.mainopt[TLEXOPT_VERSION] = 1;
					break;
				}
				else if(strcmp(long_options[opt_index].name, "help") == 0){
					printf("--help help()\n");
					conf.mainopt[TLEXOPT_HELP] = 1;
					break;
				}
				else if(strcmp(long_options[opt_index].name, "monitor") == 0){
					printf("--monitor monitoring mode!!!!!\n");
					conf.mainopt[TLEXOPT_MONITOR] = 1;
					break;
				}


				/* get argment */
				else if(strcmp(long_options[opt_index].name, "interface") == 0){
					printf("--interface set if\n");
					strncpy(conf.ifname, optarg, sizeof(conf.ifname));
					break;
				}
				else if(strcmp(long_options[opt_index].name, "timeout") == 0){
					printf("--timeout set timeout\n");
					conf.timeout = atoi(optarg);
					break;
				}
				else if(strcmp(long_options[opt_index].name, "loop") == 0){
					printf("--loop loopcount\n");
					conf.scanLoopCount = atoi(optarg);
					break;
				}
				else if(strcmp(long_options[opt_index].name, "file") == 0){
					printf("--file select logfile\n");
					strcpy(conf.logname, optarg);
					break;
				}





			/* dont get argument */
			case 'h':
				printf("-h usage\n");
				conf.mainopt[TLEXOPT_HELP] = 1;
				break;
			case 'v':
				printf("-v version()\n");
				conf.mainopt[TLEXOPT_VERSION] = 1;
				break;
			case 'p':
				printf("-p print log\n");
				conf.mainopt[TLEXOPT_PRINTLOG] = 1;
				break;
			case 's':
				printf("-s sort log\n");
				conf.mainopt[TLEXOPT_SORTLOG] = 1;
				break;


			/* get argment */
			case 'i':
				printf("-i set interface\n");
				strncpy(conf.ifname, optarg, sizeof(conf.ifname));
				break;
			case 'l':
				printf("-l set loopcount\n");
				conf.scanLoopCount = atoi(optarg);
				break;
			case 't':
				printf("-t set timeout\n");
				conf.timeout = atoi(optarg);
				break;
			case 'f':
				printf("-f select logfile\n");
				strcpy(conf.logname, optarg);
				break;
		}
	}


	
}



int main(int argc, char** argv){
	struct timeval s, e;
	clock_t startTime, endTime;
	TLexOps conf;
	


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
	else if(conf.mainopt[TLEXOPT_MONITOR]){
		MonitorLan(conf);
		return 1;	
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




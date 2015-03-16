
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
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <getopt.h>

#include <time.h>
#include "myclass.h"
#include "debug.h"



char* gettimestr(){
	char* str;
	time_t timer = time(NULL);
	struct tm *time;

	str = (char*)malloc(sizeof(char) * 256);
	memset(str, 0, sizeof str);
	time = localtime(&timer);
	strftime(str, 255, "%H:%M:%S", time);
	return str;
}




void parse_option(int argc, char** argv, TLexOps& conf){
	int opt;
	struct option long_options[] = {
		{"version", no_argument, 0, 0},
		{"help", no_argument, 0, 0},
		{"interface", required_argument, 0, 0},
		{"timeout", required_argument, 0, 0},
		{"loop", required_argument, 0, 0},
		{"file", required_argument, 0, 0},
		{"mode", required_argument, 0, 0}
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
				else if(strcmp(long_options[opt_index].name, "mode") == 0){
					printf("--mode select mode\n");
					if(strcmp(optarg, "normal") == 0)
						conf.mode = 1;
					else if(strcmp(optarg, "monitor") == 0)
						conf.mode = 2;
					else if(strcmp(optarg, "capture") == 0)
						conf.mode = 3;
					else{
						fprintf(stderr, "parse_option: mode \"%s\" not found\n", optarg);
						exit(-1);
					}
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






void printLog(const char* filename){
	FILE* fp;
	char line[100];
	unsigned int buf_id;
	char buf_live[16];
	char buf_ipaddr[16];
	unsigned int buf_mac[6];
	char buf_bender[16];
	char buf_lastchange[16];
	device buf_dev;

	if((fp=fopen(filename, "r")) == NULL){
		perror("printLog()");
		return;
	}
	
	
	printf("  +----------------");
	printf("---------------------------------------------------------+\n");
	printf("  |  %-17s  |  %-17s  |  %-13s  |  %-7s  |\n", 
			"IP address", "MAC address", "Bender", "Update");
	printf("  +----------------");
	printf("---------------------------------------------------------+\n");
	while((fgets(line, sizeof(line), fp)) != NULL){
		memset(buf_live, 0, sizeof(buf_live));
		memset(buf_ipaddr, 0, sizeof(buf_ipaddr));
		memset(buf_mac, 0, sizeof(buf_mac));
		memset(buf_bender, 0, sizeof(buf_bender));
		memset(buf_lastchange, 0, sizeof(buf_lastchange));
		
		sscanf(line, "%u %s %s %x:%x:%x:%x:%x:%x %s %s", 
				&buf_id, buf_live, buf_ipaddr, 
				&buf_mac[0],&buf_mac[1],&buf_mac[2],
				&buf_mac[3],&buf_mac[4],&buf_mac[5],
				buf_bender, buf_lastchange);
		

		if(strcmp("UP", buf_live) == 0)	buf_dev.live=true;
		else							buf_dev.live=false;
		for(int i=0; i<6; i++)	buf_dev.ha[i] = buf_mac[i];
		buf_dev.pa = inet_addr(buf_ipaddr);
		buf_dev.bender = buf_bender;
		buf_dev.lastchange = buf_lastchange;
	
		//buf_dev.showinfo();


		printf("  |  %s   \t|  ",addrtostr((unsigned int)buf_dev.pa));
		for(int i=0; i<6; i++){
			printf("%02x", buf_dev.ha[i]);
			if(i<5)	fputc(':', stdout);
			//else	fputc('\t', stdout);
		}
		printf("  | %s   \t|  ", buf_dev.bender.c_str());
		printf("%s |\n", buf_dev.lastchange.c_str());
	}
	printf("  +----------------");
	printf("---------------------------------------------------------+\n");
}



void sortLog(const char* filename){
	FILE* fp;
	char line[100];
	unsigned int buf_id;
	char buf_live[16];
	char buf_ipaddr[16];
	unsigned int buf_mac[6];
	char buf_bender[16];
	char buf_lastchange[16];
	device buf_dev;				// UseMyDataType
	std::vector<device> vec;	// UseMyDataType

	if((fp=fopen(filename, "r")) == NULL){
		perror("sortLog");
		return;
	}

	while((fgets(line, sizeof(line), fp)) != NULL){
		memset(buf_live, 0, sizeof(buf_live));
		memset(buf_ipaddr, 0, sizeof(buf_ipaddr));
		memset(buf_mac, 0, sizeof(buf_mac));
		memset(buf_bender, 0, sizeof(buf_bender));
		memset(buf_lastchange, 0, sizeof(buf_lastchange));
		
		sscanf(line, "%u %s %s %x:%x:%x:%x:%x:%x %s %s", 
				&buf_id, buf_live, buf_ipaddr, 
				&buf_mac[0],&buf_mac[1],&buf_mac[2],
				&buf_mac[3],&buf_mac[4],&buf_mac[5],
				buf_bender, buf_lastchange);
		

		// UseMyDataType
		if(strcmp("UP", buf_live) == 0)	buf_dev.live=true;
		else							buf_dev.live=false;
		for(int i=0; i<6; i++)	buf_dev.ha[i] = buf_mac[i];
		buf_dev.pa = inet_addr(buf_ipaddr);
		buf_dev.bender = buf_bender;
		buf_dev.lastchange = buf_lastchange;
	

		vec.push_back(buf_dev);		// UseMyDataType
	}
	
	// sort logs
	for(int i=0; i<vec.size()-1; i++){
		for(int j=vec.size()-1; j>i; j--){
			if(vec[j-1] > vec[j])
				std::swap(vec[j-1], vec[j]);	
		}
	}
	
	// write sorted log
	if((fp = fopen(filename, "w")) == NULL){
		perror("sortLog");
		return;
	}
	for(int i=0; i<vec.size(); i++){
		vec[i].writeLog(filename, 0);	// UseMyDataType
	}

	fclose(fp);
}




static const uint32_t FNV_OFFSET_BASIS_32 = 2166136261U;
static const uint32_t FNV_PRIME_32 = 16777619U;

uint32_t hash(uint8_t *bytes, size_t length){
    uint32_t h;
    size_t i;

    h = FNV_OFFSET_BASIS_32;
    for( i = 0 ; i < length ; ++i) {
        h = (FNV_PRIME_32 * h) ^ (bytes[i]);
    }

    return h;
}



std::vector<unsigned int> getidbylogfile(){
	FILE *fp;
	char line[100];
	unsigned int buf;
	std::vector<unsigned int> vec;

	if((fp=fopen("test.log", "r")) == NULL){
		perror("load log");
		return vec;
	}
	while(fgets(line, sizeof(line), fp) != NULL){
		sscanf(line, "%d", &buf);
		vec.push_back(buf);	
	}


	fclose(fp);
	return vec;
}



void usage(int argc, char **argv){	
	printf("usage: %s\t[-i interface] [-l loop_count]\n", argv[0]);
	printf("\t\t\t[-t timeout] [-p filename] [-s filename]\n");
	printf("\t\t\t[-f filename] [-n noverbose]\n");
	
	printf("\n");
	printf("\t-i	select interface\n");
	printf("\t-l	loop count of send_arp_to_alladdr\n");
	printf("\t-t	timeout\n");
	printf("\t-f	select logfile if file not found then make newfile\n");
	printf("\t-p	print logfile only\n");
	printf("\t-s	sort logfile only\n");
	printf("\t-n	no verbose\n");
//	printf("\t-s	\n");
//	printf("\t-s	\n");

	return;
}



void version(){
	printf("PROGRAMNAME version 0.10 \n\n");
	printf("Copyright (C) 2014-2015 Hiroki Shirokura <mail: slank.dev@gmail.com>\n");
	//printf("\n");
	printf("This file is part of PROGRAMNAME.\n");
	//printf("\n");
	printf("PROGRAMNAME is free software: you can redistribute it and/or modify\n");
	printf("it under the terms of the GNU General Public License as published by\n");
	printf("the Free Software Foundation, either version 3 of the License, or\n");
	printf("(at your option) any later version.\n");
	//printf("\n");
	printf("PROGRAMNAME is distributed in the hope that it will be useful,\n");
	//printf("\n");
	printf("but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
	printf("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
	printf("GNU General Public License for more details.\n");
	//printf("\n");
	printf("You should have received a copy of the GNU General Public License\n");
	printf("along with this program.  \n");
	printf("If not, see <http://slankdev.wordpress.com>.\n");
	printf("slank (Hiroki Shirokura) <mail: slank.dev@gmail.com>\n");

}


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
#include <sys/types.h>
#include <vector>
#include <string>

#include "addr.h"


#ifndef INCLUDED_MYCLASS
#define INCLUDED_MYCLASS



#define TLEXOPTS		100

/* TLex Original Option Code */
enum TLexOpcode{ 
	TLEXOPT_MONITOR,
	TLEXOPT_HELP,	
	TLEXOPT_VERSION, 
	TLEXOPT_PRINTLOG, 
	TLEXOPT_SORTLOG
};

class TLexOps{
	public:
		char ifname[32];
		int scanLoopCount;
		int timeout;
		char logname[64];
		int verbose;

		/*TLEx Original Option Code*/
		int mainopt[TLEXOPTS];

	
	TLexOps(){
		strcpy(ifname, "wlan0");
		scanLoopCount = 1;
		timeout = 5;
		strcpy(logname, "test.log");
		verbose = 1;
		memset(mainopt, 0, sizeof(mainopt));
	}


	void showConfig(){
		printf("--------------------------------\n");
		printf("Interface    :  %-10s     \n", ifname);
		printf("scan_count   :  %-10d     \n", scanLoopCount);
		printf("timeout      :  %-3d sec  \n", timeout);
		printf("logfile      :  %-10s     \n", logname);
		printf("verbose info :  %-10d     \n", verbose);
		printf("--------------------------------\n");
	}
};





class device{
	public:
		u_int32_t pa;
		u_char ha[6];
		std::string bender;
		std::string hostname;
		std::string comment;
		bool live;
		unsigned int id;
		std::vector<unsigned int> log;
		//time info
		
		device(){}

		device(const device &c){
			live = c.live;
			for(int i=0; i<6; i++)	ha[i] = c.ha[i];
			pa = c.pa;
			bender = c.bender;
			hostname = c.hostname;
		}
		//operator <, >, ==
		bool operator<(device dev){
			union data{
				u_char c[4];
				u_int32_t l;
			};
			data d1, d2;
			d1.l = pa;
			d2.l = dev.pa;
			
			for(int i=0; i<4; i++){
				if(d1.c[i] == d2.c[i])	continue;
				if(d1.c[i] < d2.c[i])		return true;
				else if(d1.c[i] > d2.c[i])	return false;
			}
			//return false;
		}
		bool operator>(device dev){
			union data{
				u_char c[4];
				u_int32_t l;
			};
			data d1, d2;
			d1.l = pa;
			d2.l = dev.pa;
			
			for(int i=0; i<4; i++){
				if(d1.c[i] == d2.c[i])	continue;
				if(d1.c[i] > d2.c[i])	return true;
				if(d1.c[i] < d2.c[i])	return false;
			}
			return false;
		}

		bool operator==(device dev){
			if(dev.pa == pa)	return true;
			else				return false;
		}
		bool operator!=(device dev){
			if(dev.pa != pa)	return true;
			else				return false;
		}


		void showinfo(){
			printf(" %s\t", (live==true)?"UP" : "DOWN");
			printf("  %s\t[",addrtostr((unsigned int)pa));
			for(int i=0; i<6; i++){
				printf("%02x", ha[i]);
				if(i<5)	fputc(':', stdout);
				//else	fputc('\t', stdout);
			}
			printf("(%s)]\t", bender.c_str());
			printf("%s\n", hostname.c_str());
		}
		void writeLog(const char* filename, int verbose=1){

			getid();
			loadLog(filename);
					
			for(int i=0; i<log.size(); i++){
				if(id == log[i]){
					if(verbose == 1)
						printf(" - found this log\n");
					return;
				}
			}

			FILE *fp;
			if((fp=fopen(filename, "a")) == NULL){
				perror("write log");
				return;
			}	
			
			fprintf(fp, "%u ", id);
			fprintf(fp, "%s ", (live==true)?"UP" : "DOWN");
			fprintf(fp, "%s ",addrtostr((unsigned int)pa));
			for(int i=0; i<6; i++){
				fprintf(fp, "%02x", ha[i]);
				if(i<5)	fputc(':', fp);
				else	fputc(' ', fp);
			}
			fprintf(fp, "%s ", bender.c_str());
			fprintf(fp, "%s\n", hostname.c_str());
			fclose(fp);
			
			if(verbose==1){
				printf(" - add new log [%s, ", addrtostr((unsigned int)pa));
				for(int i=0; i<6; i++){
					printf("%02x", ha[i]);
					if(i<5)	fputc(':', stdout);
					else	printf(", %u]\n", id);
				}
			}
		}

		void loadLog(const char* filename){
			FILE *fp;
			char line[100];
			unsigned int buf;

			log.clear();

			if((fp=fopen(filename, "r")) == NULL){
				fprintf(stderr, "loadLog: logfile not found make newfile\n");
				return;
			}

			while(fgets(line, sizeof(line), fp) != NULL){
				sscanf(line, "%d", &buf);
				log.push_back(buf);
			}
			/*
			printf("-------------------\n");
			for(int i=0; i<log.size(); i++)
				printf("[debug load_log_indev] id: %d\n", log[i]);
			printf("-------------------\n");
			*/

			fclose(fp);
			return;
		}

		void getid(){
			unsigned int data=0;

			data = (unsigned int)pa;
			for(int i=0; i<6; i++){
				data += (unsigned int)ha[i];	
			}
			
			//printf("id: %u\n", data);
			id = data;
			return 	;
		}
};
#endif

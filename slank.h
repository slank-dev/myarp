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
#include <sys/types.h>

#include <vector>
#include <string>

#include "util.h"

#define LOGFILE_NAME "test.log"



//#define DEBUG_getaddrsinlan
//#define DEBUG_getbenderbymac
//#define DEBUG_send_arp_request
//#define DEBUG_send_ArpReqest_AllAddr


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
			//printf("copy constracter!!\n");
			
			live = c.live;
			for(int i=0; i<6; i++)	ha[i] = c.ha[i];
			pa = c.pa;
			bender = c.bender;
			hostname = c.hostname;
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

		void writeLog(){
			unsigned int data=0;
			data = (unsigned int)pa;
			for(int i=0; i<6; i++)	data += (unsigned int)ha[i];	
			id = data;
			
			loadLog();
			
			for(int i=0; i<log.size(); i++){
				if(id == log[i]){
					printf(" - found this log\n");
					return;
				}
			}

			FILE *fp;
			if((fp=fopen(LOGFILE_NAME, "a")) == NULL){
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

			printf(" - add new log [%s, ", addrtostr((unsigned int)pa));
			for(int i=0; i<6; i++){
				printf("%02x", ha[i]);
				if(i<5)	fputc(':', stdout);
				else	printf(", %u]\n", id);
			}
			

		}
		void loadLog(){
			FILE *fp;
			char line[100];
			unsigned int buf;

			log.clear();

			if((fp=fopen("test.log", "r")) == NULL){
				perror("load_log");
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

		unsigned int getid(){
			unsigned int data=0;

			data = (unsigned int)pa;
			for(int i=0; i<6; i++){
				data += (unsigned int)ha[i];	
			}
			
			//printf("id: %u\n", data);
			id = data;
			return 	4;
		}
};


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

#include <time.h>
#include <sys/time.h>

#include "addr.h"


#ifndef INCLUDED_MYCLASS
#define INCLUDED_MYCLASS

#define TLEX_INFOPATH "TLEX.tlx"




class mytimer{
	private:
		struct timeval start;
		struct timeval end;
		
		bool sflag;
		bool eflag;

	public:

		mytimer(){ 
			sflag = false;
			eflag = false;
			mytimer_start(); 
		}
		void mytimer_start(){
			gettimeofday(&start, NULL);
			sflag = true;
		}
		void mytimer_end(){
			gettimeofday(&end, NULL);
			eflag = true;
		}
		char* intervalStr(){
			if(!(sflag && eflag))
				return NULL;
			
			char *str;
			str = (char*)malloc(sizeof(char) * 128);
			memset(str, 0, sizeof(char)*128);
			sprintf(str, "%.03lf", 
					(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)*1.0E-6);	
			return str;
		}
};





class target{
	public:
		u_char ha[6];
		u_int32_t pa;
		int port;

		target(){}
		target(u_char iha[6], u_int32_t ipa, int iport){
			for(int i=0; i<6; i++)
				ha[i] = iha[i];
			pa = ipa;
			port = iport;
		}
};





class TLexInfo{
	public:
		char name[32];
		char version[16];
		char url[64];
		char buf[256];
		char buf_item[256];
		char buf_content[256];



		TLexInfo(){		/* load file "TLEX" */
			FILE *fp;

			if((fp=fopen(TLEX_INFOPATH, "r")) == NULL){
				perror("TLexinfo()");
				strcpy(name, "TLex");
				strcpy(version, "0.0.0");
				strcpy(url, "http://www.tlex.org");
				return;
			}
			
			while(fgets(buf, sizeof(buf), fp) != NULL){	
				memset(buf_item, 0, sizeof buf_item);
				memset(buf_content, 0, sizeof buf_content);

				sscanf(buf, "%s = %s\n", buf_item, buf_content);
				if(buf_item[0] == '#')	/* comment row */
					continue;	

				else if(strcmp(buf_item, "TLEX_NAME") == 0)
					strcpy(name, buf_content);
				else if(strcmp(buf_item, "TLEX_VERSION") == 0)
					strcpy(version, buf_content);
				else if(strcmp(buf_item, "TLEX_URL") == 0)
					strcpy(url, buf_content);
			}

#ifdef DEBUG_class_TLexInfo
			printf("\n");
			printf("[DEBUG] in function \"%s\" %s:%d \n", 
							__func__, __FILE__, __LINE__);
			printf("name   : %s\n", name);
			printf("version: %s\n", version);
			printf("url    : %s\n", url);
			printf("\n");
#endif

		}




		char *str(){
			char *str;
			str = (char*)malloc(sizeof(char)*256);
			sprintf(str, "%s %s ( %s )", name, version, url);
			return str;
		}

};






enum TLexModecode{	/* Tlex Originam Mode Code */
	TLEXMODE_SCAN_NORMAL,
	TLEXMODE_SCAN_MONITOR,
	TLEXMODE_SCAN_LONG,
	TLEXMODE_HELP,	
	TLEXMODE_VERSION, 
	TLEXMODE_PRINTLOG, 
	TLEXMODE_SORTLOG
};
enum TLexVerbosecode{
	TLEXVERBOSE_OFF,
	TLEXVERBOSE_ON,
	TLEXVERBOSE_SEMI		/* for mode "long" & "monitor" */ 
};

#define TLEXOPTS		100
class TLexOps{ 
	public:
		char ifname[32];
		int scanLoopCount;
		int timeout;
		char logname[64];

		int verbose;	// enum TLexVerbosecode
		int mode;	// enum TLexModecode


	

	TLexOps(){
		strcpy(ifname, "wlan0");
		scanLoopCount = 1;
		timeout = 5;
		strcpy(logname, "test.log");
		verbose = 1;
		mode = 1;
	}

	void showMode(){
		printf("mode: ");

		switch(mode){
			case TLEXMODE_SCAN_NORMAL:
				printf("normal");
				break;
			case TLEXMODE_SCAN_MONITOR:
				printf("monitor");
				break;
			case TLEXMODE_SCAN_LONG:
				printf("long");
				break;
			case TLEXMODE_HELP:
				printf("help");
				break;
			case TLEXMODE_VERSION:
				printf("version");
				break;
			case TLEXMODE_PRINTLOG:
				printf("printLog");
				break;
			case TLEXMODE_SORTLOG:
				printf("sortLog");
				break;
		}

		printf("\n");
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
		std::string lastchange;
		bool live;
		unsigned int id;
		std::vector<unsigned int> logs;

		int countIP;





		
		device(){ countIP = 0; }



		device(const device &c){
			live = c.live;
			for(int i=0; i<6; i++)	ha[i] = c.ha[i];
			pa = c.pa;
			bender = c.bender;
			lastchange = c.lastchange;
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
			printf("%s\t",addrtostr((unsigned int)pa));
			for(int i=0; i<6; i++){
				printf("%02x", ha[i]);
				if(i<5)	fputc(':', stdout);
				//else	fputc('\t', stdout);
			}
			printf("(%s)\t", bender.c_str());
			printf("%s\n", lastchange.c_str());
		}




		void writeLog(const char* filename, int verbose=1){

			getid();
			loadLog(filename);
					
			for(int i=0; i<logs.size(); i++){
				if(id == logs[i]){
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
			fprintf(fp, "%s \n", lastchange.c_str());
			fclose(fp);
			
			if(verbose==1 || verbose==2){
				printf(" - Add new log at %s [%s, ", lastchange.c_str(),
						addrtostr((unsigned int)pa));
				for(int i=0; i<6; i++){
					printf("%02x", ha[i]);
					if(i<5)	fputc(':', stdout);
					else	printf(", %u]\n", id);
				}
			}
#ifdef DEBUG_class_device
			printf("\n");
			printf("[DEBUG] in function \"%s\" %s:%d ", 
							__func__, __FILE__, __LINE__);
			printf("log is %s [%s, ", lastchange.c_str(),
					addrtostr((unsigned int)pa));
			for(int i=0; i<6; i++){
				printf("%02x", ha[i]);
				if(i<5)	fputc(':', stdout);
				else	printf(", %u]\n", id);
			}
			printf("\n");
#endif
		}





		void loadLog(const char* filename){
			FILE *fp;
			char line[100];
			unsigned int buf;

			logs.clear();

			if((fp=fopen(filename, "r")) == NULL){
				fprintf(stderr, " - loadLog: logfile not found make newfile\n");
				return;
			}

			while(fgets(line, sizeof(line), fp) != NULL){
				sscanf(line, "%d", &buf);
				logs.push_back(buf);
			}

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

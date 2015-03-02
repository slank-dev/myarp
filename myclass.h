#include <vector>
#include <string>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <vector>
#include <string>

#include "addr.h"

#ifndef INCLUDED_MYCLASS
#define INCLUDED_MYCLASS




class scanLanConfig{
	public:
		char ifname[32];
		int scanLoopCount;
		int timeout;
		char logname[64];
		int verbose;
	
	scanLanConfig(){
		strcpy(ifname, "wlan0");
		scanLoopCount = 1;
		timeout = 5;
		strcpy(logname, "test.log");
		verbose = 1;
	}

	void showConfig(){
		//printf("----------------------------------\n");
		printf("Interface: %s\n", ifname);
		printf("scan_count: %d times\n", scanLoopCount);
		printf("timeout: %d sec\n", timeout);
		printf("logfile: %s\n", logname);
		printf("verbose info: %d\n", verbose);
		//printf("----------------------------------\n");
	}
};



class GeneralInfo{
	public:
		std::string version;
	
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

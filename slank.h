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




//#define DEBUG_getaddrsinlan
//#define DEBUG_getbenderbymac
//#define DEBUG_send_arp_request



class device{
	public:
		u_int32_t pa;
		u_char ha[6];
		std::string bender;
		std::string hostname;
		std::string comment;
		bool live;
		//time info

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
			printf( "write log!!\n");
			FILE *fp;
			if((fp=fopen("test.log", "a")) == NULL){
				perror("write log");
				return;
			}

			fprintf(fp, "%s\t", (live==true)?"UP" : "DOWN");
			fprintf(fp, "%s\t",addrtostr((unsigned int)pa));
			for(int i=0; i<6; i++){
				fprintf(fp, "%02x", ha[i]);
				if(i<5)	fputc(':', fp);
				else	fputc('\t', fp);
			}
			fprintf(fp, "%s\t", bender.c_str());
			fprintf(fp, "%s\n", hostname.c_str());

			fclose(fp);

		}
};

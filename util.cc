
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
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <vector>



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
	printf("usage: %s\t[-i interface] [-c loop_count]\n", argv[0]);
	printf("\t\t\t[-t timeout] [-p filename] [-s filename]\n");
	printf("\t\t\t[-f filename]\n");
	
	printf("\n");
	printf("\t-i	select interface\n");
	printf("\t-c	loop count of send_arp_to_alladdr\n");
	printf("\t-t	timeout\n");
	printf("\t-f	select logfile if file not found then make newfile\n");
	printf("\t-p	print logfile only\n");
	printf("\t-s	sort logfile only\n");
//	printf("\t-s	\n");
//	printf("\t-s	\n");

	return;
}

void version(){//[[[
	printf("PROGRAMNAME version 0.00 \n\n");
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

}//]]]

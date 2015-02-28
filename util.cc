
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


std::vector<unsigned int> return_log(){
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


void usage(){	
}

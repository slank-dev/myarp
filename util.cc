
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

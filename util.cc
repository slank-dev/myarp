
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <vector>



static const uint32_t FNV_OFFSET_BASIS_32 = 2166136261U;
static const uint32_t FNV_PRIME_32 = 16777619U;



uint32_t hash(uint8_t *bytes, size_t length){
    uint32_t hash;
    size_t i;

    hash = FNV_OFFSET_BASIS_32;
    for( i = 0 ; i < length ; ++i) {
        hash = (FNV_PRIME_32 * hash) ^ (bytes[i]);
    }

    return hash;
}

int load_log(std::vector<unsigned int> vec){
	FILE *fp;
	char line[100];
	unsigned int buf;

	if((fp=fopen("test.log", "r")) == NULL){
		perror("load log");
		return -1;
	}
	while(fgets(line, sizeof(line), fp) != NULL){
		sscanf(line, "%d", &buf);
		vec.push_back(buf);	
	//	printf("%u\n", buf);
	}

	fclose(fp);
	return vec.size();
}

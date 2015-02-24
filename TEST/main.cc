#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void getbenderbymac(const u_char data[6], char* bender){
	FILE *fp;
	const char* filename = "mac_code.txt";
	u_char dev_mac[3] = {data[0],data[1],data[2]};
	

	if((fp=fopen(filename, "r")) == NULL){
		perror("getbenderbymac fopen");
		strcpy(bender, "getbenderbymac error");
		return;
	}

	char strbuf[256];
	unsigned int  mac[3];

	while(fgets(strbuf, sizeof(strbuf), fp) != NULL){
		sscanf(strbuf, "%2x%2x%2x\t%s", &mac[0],&mac[1],&mac[2],bender);
		
		if(mac[0]==dev_mac[0]&&mac[1]==dev_mac[1]&&mac[2]==dev_mac[2]){
			//printf("%02X:%02x:%02x (%s)\n",mac[0],mac[1],mac[2],bender);
			return;
		}


		memset(mac, 0, sizeof(mac));
		memset(bender, 0, sizeof(bender));
		memset(strbuf, 0, sizeof(strbuf));
	}
	strcpy(bender, "not found");

	fclose(fp);
	
	return;
}



int main(){
	u_char mac[] = {0xf0,0x24,0x75,0xbf,0x8d,0xbf};
	char name[256];
	getbenderbymac(mac, name);
	printf("%s\n", name);
	
	

}

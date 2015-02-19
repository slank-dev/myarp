#include <stdio.h>
#include "functions.h"
#include "head.h"

int main(){
	char* str = get_paddr("eth0");
	printf("%s\n", str);
	
	u_char* strs = get_haddr("eth0");
	for (int i=0; i<6; i++)	
		printf("%02X:", strs[i]);
	printf("\n");

	return 0;
}

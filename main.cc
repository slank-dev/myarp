#include <stdio.h>
#include "functions.h"
#include "head.h"


int main(){
	char* str = get_paddr("eth0");
	printf("%-8s: %s\n", "IPaddr", str);
	
	u_char* strs = get_haddr("eth0");
	printf("%-8s: ", "MACaddr");
	for (int i=0; i<6; i++){
		printf("%02x", strs[i]);

		if (i!=5)	printf(":");
		else		printf("\n");
	}

	return 0;
}

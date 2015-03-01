
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
		printf("----------------------------------\n");
		printf("Interface: %s\n", ifname);
		printf("scan_count: %d\n", scanLoopCount);
		printf("timeout: %d\n", timeout);
		printf("logfile: %s\n", logname);
		printf("verbose info: %d\n", verbose);
		printf("----------------------------------\n");
	}
};


void sortLog(const char* filename);
void printLog(const char* filename);

int send_ArpRequest_AllAddr(scanLanConfig sconfig);
	
void recvPackHandle(u_char* nouse, const struct pcap_pkthdr* header,
					const u_char* packet);
int scanLan(scanLanConfig sconfig);

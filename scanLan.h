
/*
 *Copyright (C) 2014-2015 Hiroki Shirokura <mail: slank.dev@gmail.com>
 *
 *TLexScan is free software: you can redistribute it and/or modify
 *it under the terms of the GNU General Public License as published by
 *the Free Software Foundation, either version 3 of the License, or
 *(at your option) any later version.
 *
 *TLexScan is distributed in the hope that it will be useful,
 *
 *but WITHOUT ANY WARRANTY; without even the implied warranty of
 *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *GNU General Public License for more details.
 *
 *You should have received a copy of the GNU General Public License
 *along with this program.  
 *If not, see <http://slankdev.wordpress.com>.
 *slank (Hiroki Shirokura) <mail: slank.dev@gmail.com>
 *
 */

#include <stdio.h>
#include <string.h>

#include "myclass.h"




int pcap_init_scan(TLexOps sconfig);

void ScanCallback(u_char* nouse, 
		const struct pcap_pkthdr* header,const u_char* packet);
int ScanLan(TLexOps sconfig);

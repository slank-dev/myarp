# TLexScan 
The whole area of LAN examination Scaner  
Local area network scaner for linux from slank  

## Description
TLexScan looks for all devices  in the LAN, 
you can create a list by examining their IP and MAC.  
The survival confirmation of the device, I am using the arp.
User will be able to know the information of the LAN 
that is quickly and easily and safely your connection .  

## Demo
 makeing now  
 comming soon  

## VS.
 Fing <http://www.overlooksoft.com/fing>  

## Requirement
 libpcap <http://www.tcpdump.org>  
 gcc (Ubuntu 4.8.2-19ubuntu1) 4.8.2 <http://gcc.gnu.org>  
 GNU Make 3.81 <http://www.gnu.org/software/make>  

## Usage

	usage: ./tlex.out	[-i interface] [-c loop_count]
				[-t timeout] [-p filename] [-s filename]
				[-f filename] [-n noverbose]

		-i	select interface
		-c	loop count of send_arp_to_alladdr
		-t	timeout
		-f	select logfile if file not found then make newfile
		-p	print logfile only
		-s	sort logfile only
		-n	no verbose


## Contribution
1. Fork it ( $ git clone http://github.com/slank-dev/tlexscan.git )
2. Create your feature branch (git checkout -b my-new-feature)
3. Commit your changes (git commit -am 'Add some feature')
4. Push to the branch (git push origin my-new-feature)


## Instration
	 
	$ make
	g++    -c -o main.o main.cc
	g++    -c -o addr.o addr.cc
	g++    -c -o arp.o arp.cc
	g++    -c -o scanLan.o scanLan.cc
	g++    -c -o util.o util.cc
	g++  main.o addr.o arp.o scanLan.o util.o -o tlex.out -Wall -lpcap


	$ sudo ./tlex.out 
	Starting TLexScan 0.00 Interface: wlan0
	scan_count: 1 times
	timeout: 5 sec
	logfile: test.log
	verbose info: 1

	[ArpSend in LAN Started] 
	 - found this log
	 - found this log
	 - add new log [192.168.179.2, f0:24:75:bf:8d:bf, 45329492]
	[Scan Finished]

	 UP	  192.168.179.1	[a2:12:42:17:d8:8f(not-found)]	aterm.me
	 UP	  192.168.179.2	[f0:24:75:bf:8d:bf(Apple)]	
	 UP	  192.168.179.4	[80:e6:50:17:18:46(Apple)]	
	
	$ 
 

## Licence
 
	Copyright (C) 2014-2015 Hiroki Shirokura <mail: slank.dev@gmail.com>

	TLexScan is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	TLexScan is distributed in the hope that it will be useful,

	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  
	If not, see <http://slankdev.wordpress.com>.
	slank (Hiroki Shirokura) <mail: slank.dev@gmail.com>

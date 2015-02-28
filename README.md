# send_pack
local area network scaner for linux from slank

## instration
	$ make
	$ sudo ./nscan.out
	[ArpSend in LAN Started] 
	load_log: No such file or directory
	 - add new log [192.168.179.1, a2:12:42:aa:aa:aa, 28551988]
	 - add new log [192.168.179.2, f0:24:75:bb:bb:bb, 45329492]
	 - add new log [192.168.179.4, 80:e6:50:cc:cc:cc, 78883563]
     - add new log [192.168.179.3, 84:b1:53:dd:dd:dd, 62106369]
	 - add new log [192.168.179.8, cc:e1:d5:ee:ee:ee, 145992739]
	[Scan Finished]
	----------------------------------------------------------
	 UP	 192.168.179.1	[a2:12:42:aa:aa:aa(not-found)]	aterm.me
	 UP  192.168.179.2	[f0:24:75:bb:bb:bb(Apple)]	
	 UP  192.168.179.4	[80:e6:50:cc:cc:cc(Apple)]	
	 UP  192.168.179.3	[84:b1:53:dd:dd:dd(Apple)]	
	 UP  192.168.179.8	[cc:e1:d5:ee:ee:ee(Buffalo)]	ubuntu
 	----------------------------------------------------------
	$ make clean

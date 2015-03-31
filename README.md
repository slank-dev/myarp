# TLex - network scaner 
TLexScan version 0.10 -The whole area of LAN examination Scaner  
LANに接続されているデバイス情報を調べるプログラム

## 概要
無線LANに接続しているデバイスすべてを調べることができるプログラム。
nic情報からipアドレスとネットマスクを調べ、そこからそのローカルネットワークに存在しうるアドレス全てに対して
arpパケットを送り生存確認をしてMACアドレスも調べます。
得られた情報からipアドレスとMACアドレスの対応表を作り標準出力に表示させます。
スキャン方法や細かいタイムアウト設定などをオプションなどで柔軟に対応させられるようにしています。


## デモ
 youtubeにでも動画をアップロードしているのでそちらを参照してください
 http://youtu.be/X75uHbJuGr4


## 似たソフトウェア
 Fing <http://www.overlooksoft.com/fing>   

## 開発ツール、ライブラリ
 libpcap <http://www.tcpdump.org>   
 gcc (Ubuntu 4.8.2-19ubuntu1) 4.8.2 <http://gcc.gnu.org>  
 GNU Make 3.81 <http://www.gnu.org/software/make>  

## 使用方法
 
	usage: ./tlex.out	[-i interface] [-l loop_count]
				[-t timeout] [-p filename] [-s filename]
				[-f filename] [-n noverbose]

		-h   help message
		-v   version
		-i [interface]   select interface
		-l [loop count]   loop count of send_arp_to_alladdr
		-t [timeout]   timeout
		-f [filename]   select logfile if file not found then make newfile
		-p [filename]   print logfile only
		-s [filename]   sort logfile only

		--version   show version
		--help   show help
		--interface   select interface
		--timeout   set timeout
		--loop   set scanLoop 
		--file   select log file
		--verbose   verbose mode 
		--noverbose   noverbose mode 
		--mode [ normal | long | monitor ]   select mode


## Contribution
1. Fork it ( $ git clone http://github.com/slank-dev/tlexscan.git )
2. Create your feature branch (git checkout -b my-new-feature)
3. Commit your changes (git commit -am 'Add some feature')
4. Push to the branch (git push origin my-new-feature)

 
## インストレーション
	 
	$ make
	g++    -c -o main.o main.cc
	g++    -c -o addr.o addr.cc
	g++    -c -o arp.o arp.cc
	g++    -c -o scanLan.o scanLan.cc
	g++    -c -o util.o util.cc
	g++  main.o addr.o arp.o scanLan.o util.o -o tlex.out -Wall -lpcap
	

	$ sudo ./tlex.out -i eth0
	Starting TLex 0.1.0 ( http://www.tlex.org ) 02:27:54
	--------------------------------
	Interface    :  eth0           
	scan_count   :  1              
	timeout      :  5   sec  
	logfile      :  test.log       
	verbose info :  1              
	--------------------------------
	 * [ARPSCAN] start
	 - Add new log at 02:27:55 [192.168.179.6, 80:e6:50:17:18:46, 112437995]
	 - Add new log at 02:27:55 [192.168.179.1, a2:12:42:17:d8:8f, 28551988]
	 - Add new log at 02:27:55 [192.168.179.15, b8:27:eb:34:6b:92, 263433147]
	 - Add new log at 02:27:55 [192.168.179.4, 84:b1:53:6a:2f:20, 78883585]
	 * [ARPSCAN] finish

	  +-------------------------------------------------------------------------+
	  |  IP address         |  MAC address        |  Bender         |  Update   |
	  +-------------------------------------------------------------------------+
	  |  192.168.179.1   	|  a2:12:42:17:d8:8f  | not-found      	|  02:27:55 |
	  |  192.168.179.4   	|  84:b1:53:6a:2f:20  | Apple      	|  02:27:55 |
	  |  192.168.179.6   	|  80:e6:50:17:18:46  | Apple      	|  02:27:55 |
	  |  192.168.179.15   	|  b8:27:eb:34:6b:92  | Raspberry      	|  02:27:55 |
	  +-------------------------------------------------------------------------+

	TLex done: scan finished in 5.824 sec 
	$
 
## ライセンス
  
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

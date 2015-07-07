#makefile

export CC  := gcc
export CPP := g++ 
export CXX := g++ 
export RM  := rm -rf
export MKDIR := mkdir -p


.SUFFIXES: .out .c .cc .o .h 


program = tlex
SRC = main.cc \
	  addr.cc \
	  arp.cc \
	  scanLan.cc\
	  util.cc \
	  lscanLan.cc\
	  monLan.cc
HDR = addr.h\
	  arp.h\
	  debug.h\
	  lscanLan.h\
	  monLan.h\
	  myclass.h\
	  scanLan.h\
	  util.h
OBJ = $(SRC:.cc=.o)
CFLAGS = -Wall -lpcap -pthread
LIBPCAPDIR = libpcap






all: $(program)


$(program): $(OBJ)
	$(CPP) $(OBJ) -o $(program) $(CFLAGS)


.cc.o: 
	$(CPP) -std=c++11 -c  $<  -o $@  



#----------------------------------OTHERS--------------------------------------#



clean:
	$(RM) *.o *.ouat
info:
distclean:
check:

version:
	echo  $(MAKE_VERSION)



#----------------------------------OTHERS--------------------------------------#




allclean:
	sudo $(RM) -f  *.o *.out *.log


install:
	cp $(program) /usr/local/bin





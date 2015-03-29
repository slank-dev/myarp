#makefile

CPP := g++ 
CC  := gcc
RM  := rm -rf
MKDIR := mkdir -p



program = tlex.out

SRC = main.cc addr.cc arp.cc scanLan.cc util.cc lscanLan.cc monLan.cc
OBJ = $(SRC:.cc=.o)
HDR = addr.h arp.h debug.h lscanLan.h monLan.h myclass.h scanLan.h util.h


CFLAGS = -Wall -lpcap -pthread

.SUFFIXES: .out .c .cc .o .h 



all: $(program)

$(program): $(OBJ)
	$(CPP) $(OBJ) -o $(program) $(CFLAGS)


.cc.o: 
	$(CPP) -std=c++11 -c  $<  -o $@  



install:
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



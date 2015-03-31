#makefile


program = tlex.out


SRC = main.cc addr.cc arp.cc scanLan.cc util.cc lscanLan.cc monLan.cc
OBJ = $(SRC:.cc=.o)
CFLAGS = -Wall -lpcap -pthread
CPP = g++ 

all: $(program)

$(program): $(OBJ)
	$(CPP) $(OBJ) -o $(program) $(CFLAGS)


.cc.o: 
	$(CPP) -c -std=c++11 $<



clean:
	rm *.o *.out


allclean:
	sudo rm *.o *.out *.log

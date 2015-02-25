#makefile

program = nscan.out

SRC = main.cc addr.cc arp.cc scanLan.cc
OBJ = $(SRC:.cc=.o)
CFLAGS = -Wall -lpcap
CPP = g++ 

all: $(program)

$(program): $(OBJ)
	$(CPP) $(OBJ) -o $(program) $(CFLAGS)


clean:
	rm *.o *.out

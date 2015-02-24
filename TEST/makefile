#makefile

program = arpscan.out

SRC = main.cc addr.cc arp.cc 
OBJ = $(SRC:.cc=.o)
CFLAGS = -Wall
CPP = g++ 

all: $(program)

$(program): $(OBJ)
	$(CPP) $(OBJ) -o $(program)


clean:
	rm *.o *.out

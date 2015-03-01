#makefile

program = tlex.out

SRC = main.cc addr.cc arp.cc scanLan.cc util.cc
OBJ = $(SRC:.cc=.o)
CFLAGS = -Wall -lpcap
CPP = g++ 

all: $(program)

$(program): $(OBJ)
	$(CPP) $(OBJ) -o $(program) $(CFLAGS)


clean:
	rm *.o *.out


allclean:
	sudo rm *.o *.out *.log

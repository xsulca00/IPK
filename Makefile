FLAGS=-std=c++11 -O0 -Wall -Wextra -pedantic -g 
CC=g++

CLI=ftrest
SVR=ftrestd
LS=ls
BITS=bitstream

all: bitstream.o ls.o client.o server.o
	$(CC) $(FLAGS) -o $(CLI) $(BITS).o $(LS).o $(CLI).o
	$(CC) $(FLAGS) -o $(SVR) $(BITS).o $(LS).o $(SVR).o

server.o: ftrestd.cpp
	$(CC) $(FLAGS) -c $(SVR).cpp

client.o: ftrest.cpp 
	$(CC) $(FLAGS) -c $(CLI).cpp

bitstream.o: bitstream.h bitstream.cpp
	$(CC) $(FLAGS) -c $(BITS).cpp

ls.o: ls.h ls.cpp
	$(CC) $(FLAGS) -c $(LS).cpp

clean:
	rm -f *.o

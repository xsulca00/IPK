FLAGS=-std=c++11 -O0 -Wall -Wextra -pedantic -g 
CC=g++

CLI=ftrest
SVR=ftrestd
LS=ls
BITS=bitstream

all: bitstream.o ls.o client.o server.o
	$(CC) -o $(CLI) $(BITS).o $(LS).o $(CLI).o
	$(CC) -o $(SVR) $(BITS).o $(LS).o $(SVR).o

server.o: ftrestd.cpp
	$(CC) -c $(SVR).cpp

client.o: ftrest.cpp 
	$(CC) -c $(CLI).cpp

bitstream.o: bitstream.h bitstream.cpp
	$(CC) -c $(BITS).cpp

ls.o: ls.h ls.cpp
	$(CC) -c $(LS).cpp

clean:
	rm -f *.o

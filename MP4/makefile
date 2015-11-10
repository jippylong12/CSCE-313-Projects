# makefile

all: dataserver simpleclient

reqchannel.o: reqchannel.h reqchannel.cpp
	g++ -std=c++11 -c -g reqchannel.cpp
	
semaphore.o: semaphore.h semaphore.cpp
	g++ -std=c++11 -c -g semaphore.cpp

boundedbuffer.o: boundedbuffer.h boundedbuffer.cpp
	g++ -std=c++11 -c -g boundedbuffer.cpp

dataserver: dataserver.cpp reqchannel.o 
	g++ -std=c++11 -g -o dataserver dataserver.cpp reqchannel.o -lpthread

simpleclient: simpleclient.cpp boundedbuffer.o reqchannel.o semaphore.o
	g++ -std=c++11 -g -o simpleclient simpleclient.cpp boundedbuffer.o reqchannel.o semaphore.o -lpthread
clean:
	rm *.o dataserver simpleclient fifo*

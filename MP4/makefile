# makefile

all: dataserver simpleclient

reqchannel.o: reqchannel.h reqchannel.cpp
	g++ -c -g reqchannel.cpp
	
semaphore.o: semaphore.h semaphore.cpp
	g++ -c -g semaphore.cpp

boundedbuffer.o: boundedbuffer.h boundedbuffer.cpp
	g++ -c -g boundedbuffer.cpp

dataserver: dataserver.cpp reqchannel.o 
	g++ -g -o dataserver dataserver.cpp reqchannel.o -lpthread

simpleclient: simpleclient.cpp boundedbuffer.o reqchannel.o semaphore.o
	g++ -g -o simpleclient simpleclient.cpp boundedbuffer.o reqchannel.o semaphore.o -lpthread
clean:
	rm *.o dataserver simpleclient

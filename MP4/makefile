# makefile

all: dataserver simpleclient

reqchannel.o: reqchannel.h reqchannel.cpp
	g++-4.7 -c -g reqchannel.cpp
	
semaphore.o: semaphore.h semaphore.cpp
	g++-4.7 -c -g semaphore.cpp

boundedbuffer.o: boundedbuffer.h boundedbuffer.cpp
	g++-4.7 -c -g boundedbuffer.cpp

dataserver: dataserver.cpp reqchannel.o 
	g++-4.7 -g -o dataserver dataserver.cpp reqchannel.o -lpthread

simpleclient: simpleclient.cpp boundedbuffer.o reqchannel.o semaphore.o
	g++-4.7 -g -o simpleclient simpleclient.cpp boundedbuffer.o reqchannel.o semaphore.o -lpthread
clean:
	rm *.o dataserver simpleclient
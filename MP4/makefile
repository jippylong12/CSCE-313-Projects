# makefile

all: dataserver simpleclient

reqchannel.o: reqchannel.h reqchannel.cpp
	g++ -c -g reqchannel.cpp

semaphore.o: semaphore.h semaphore.cpp
	g++ -c -g semaphore.cpp

dataserver: dataserver.cpp reqchannel.o 
	g++ -g -o dataserver dataserver.cpp reqchannel.o -lpthread

simpleclient: simpleclient.cpp reqchannel.o
	g++ -g -o simpleclient simpleclient.cpp reqchannel.o
# makefile

all: dataserver simpleclient client

reqchannel.o: reqchannel.h reqchannel.cpp
	g++ -std=c++11 -c -g reqchannel.cpp
	
semaphore.o: semaphore.h semaphore.cpp
	g++ -std=c++11 -c -g semaphore.cpp

boundedbuffer.o: boundedbuffer.h boundedbuffer.cpp
	g++ -std=c++11 -c -g boundedbuffer.cpp

dataserver: dataserver.cpp reqchannel.o 
	g++ -std=c++11 -g -o dataserver dataserver.cpp reqchannel.o -lpthread

simpleclient: simpleclient.cpp
	g++ -std=c++11 -g -o simpleclient simpleclient.cpp reqchannel.o -lpthread
	
client: client.cpp boundedbuffer.o reqchannel.o semaphore.o
	g++ -std=c++11 -g -o client client.cpp boundedbuffer.o reqchannel.o semaphore.o -lpthread

clean:
	rm *.o dataserver simpleclient fifo*

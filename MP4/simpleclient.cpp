/* 
    File: simpleclient.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 2013/01/31

    Simple client main program for MP3 in CSCE 313
*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cassert>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>

#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <thread>

#include "reqchannel.h"
#include "boundedbuffer.h"

using namespace std;

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/
BB request_buffer;
vector<BB> response_buffer(3);
void* RT(void* arg){ //request thread
	int person=*(int *)arg;
	if(person==0){//Joe
		for(int i=0;i<10;i++)
			request_buffer.push("data Joe Smith");
	}
	
	else if(person==1){//Jane
		for(int i=0;i<10;i++)
			request_buffer.push("data Jane Smith");
	}
	
	else if (person==2){//John
		for(int i=0;i<10;i++)
			request_buffer.push("data John Doe");
	}
	
		//cerr<<"Something went wrong request buffer";
	
}

void* WT(void* arg){
	RequestChannel control=*(RequestChannel *)arg;
	string channel_name=control.send_request("newthread");
	RequestChannel channel_req=RequestChannel(channel_name,RequestChannel::CLIENT_SIDE);
	while(true){
		string nq=request_buffer.pop(); // pulls request from buffer
		cerr<<"IN WT4";
		string resp=channel_req.send_request(nq);
		if(nq.compare(5,3,"Joe"))
			response_buffer[0].push(resp);
		else if(nq.compare(5,4,"Jane"))
			response_buffer[1].push(resp);
		else if(nq.compare(5,4,"John"))
			response_buffer[2].push(resp);
		
			//cerr<<"something went wrong in the worker thread";
		
	}
	
}

int main(int argc, char * argv[]) {

    pid_t pid = fork();
    //if child process
    //we need to start up the data server
    if(pid == 0) 
    {
        execvp("./dataserver",argv);
    }
	else
	{
		
		int size=9000;
		request_buffer= BB(size);
		for(int i=0;i<3;i++)
			response_buffer[i]= BB(size/3);
		
		
		cout << "CLIENT STARTED:" << endl;

		cout << "Establishing control channel... " << flush;
		RequestChannel chan("control", RequestChannel::CLIENT_SIDE);
		cout << "done." << endl;
		int w=5;
		pthread_t tid;
		/* -- Start sending a sequence of requests */
		for (int i=0; i<3;i++)
		{
			pthread_create (&tid,0, RT, new int(i)); // arguments you want to call this function with
			
		}
		request_buffer.isempty();
		for(int i=0;i<w;i++)
			pthread_create(&tid, 0, WT , &chan);

		pthread_join(tid,NULL);
		/*   string reply1 = chan.send_request("hello");
		cout << "Reply to request 'hello' is '" << reply1 << "'" << endl;

		string reply2 = chan.send_request("data Joe Smith");
		cout << "Reply to request 'data Joe Smith' is '" << reply2 << "'" << endl;

		string reply3 = chan.send_request("data Jane Smith");
		cout << "Reply to request 'data Jane Smith' is '" << reply3 << "'" << endl;

		string reply5 = chan.send_request("newthread");
		cout << "Reply to request 'newthread' is " << reply5 << "'" << endl;
		RequestChannel chan2(reply5, RequestChannel::CLIENT_SIDE);

		string reply6 = chan2.send_request("data John Doe");
		cout << "Reply to request 'data John Doe' is '" << reply6 << "'" << endl;

		string reply7 = chan2.send_request("quit");
		cout << "Reply to request 'quit' is '" << reply7 << "'" << endl;
*/
		string reply4 = chan.send_request("quit");
		cout << "Reply to request 'quit' is '" << reply4 << "'" << endl;
		
		usleep(1000000);
		}
}

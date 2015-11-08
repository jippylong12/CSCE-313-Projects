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

int main(int argc, char * argv[]) {

    pid_t pid = fork();
    //if child process
    //we need to start up the data server
    if(pid == 0) 
    {
        execvp("./dataserver",argv);
    }



  cout << "CLIENT STARTED:" << endl;

  cout << "Establishing control channel... " << flush;
  RequestChannel chan("control", RequestChannel::CLIENT_SIDE);
  cout << "done." << endl;
  pid_t id[3];
/*   for(int i=0; i<3;i++){
	  pthread_create(&id,0,RT,&i)
  } */
  /* -- Start sending a sequence of requests */

  string reply1 = chan.send_request("hello");
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

  string reply4 = chan.send_request("quit");
  cout << "Reply to request 'quit' is '" << reply4 << "'" << endl;

  usleep(1000000);
}
/* void RT(void *arg)
{
	int person=*(int *)arg;
	 string reply5 = chan.send_request("newthread");
	cout << "Reply to request 'newthread' is " << reply5 << "'" << endl;
	RequestChannel chan2(reply5, RequestChannel::CLIENT_SIDE);
}

void WT(void* arg){
	string channel_name=control.send_request("new thread");
	RequestChannel channel_ret=RequestChannel(channel_name,RequestChannel::CLIENT_SIDE);
	while(true){
		string nq=BB.pull(); // pulls request from buffer
		string resp=channel_ret.send
	}
} */

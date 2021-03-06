/* 
    File: dataserver.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 2012/07/16

    Dataserver main program for MP3 in CSCE 313
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
#include <sstream>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>

#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#include "reqchannel.h"
#include "NetworkRequestChannel.h"

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
/* VARIABLES */
/*--------------------------------------------------------------------------*/

static int nthreads = 0;

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

void* handle_process_loop(RequestChannel & _channel);

/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS -- SUPPORT FUNCTIONS */
/*--------------------------------------------------------------------------*/

string int2string(int number) {
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS -- THREAD FUNCTIONS */
/*--------------------------------------------------------------------------*/

void * handle_data_requests(void * args) {

  RequestChannel * data_channel =  (RequestChannel*)args;

  // -- Handle client requests on this channel. 
  
  handle_process_loop(*data_channel);

  // -- Client has quit. We remove channel.
 
  delete data_channel;
}

/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS -- INDIVIDUAL REQUESTS */
/*--------------------------------------------------------------------------*/

void process_hello(RequestChannel & _channel, const string & _request) {
  _channel.cwrite("hello to you too");
}

void process_data_Joe(RequestChannel & _channel, const string &  _request) {
  usleep(1000 + (rand() % 5000));
  //_channel.cwrite("here comes data about " + _request.substr(4) + ": " + int2string(random() % 100));
  _channel.cwrite(int2string(rand() % 100));
}

void process_data_Jane(RequestChannel & _channel, const string &  _request) {
  usleep(1000 + (rand() % 5000));
  //_channel.cwrite("here comes data about " + _request.substr(4) + ": " + int2string(random() % 100));
  _channel.cwrite(int2string(rand() % 100));
}

void process_data_John(RequestChannel & _channel, const string &  _request) {
  usleep(1000 + (rand() % 5000));
  //_channel.cwrite("here comes data about " + _request.substr(4) + ": " + int2string(random() % 100));
  _channel.cwrite(int2string(rand() % 100));
}


void process_newthread(RequestChannel & _channel, const string & _request) {
  int error;
  nthreads ++;

  // -- Name new data channel

  string new_channel_name = "data" + int2string(nthreads) + "_";
  //  cout << "new channel name = " << new_channel_name << endl;

  // -- Pass new channel name back to client

  _channel.cwrite(new_channel_name);

  // -- Construct new data channel (pointer to be passed to thread function)
  
  RequestChannel * data_channel = new RequestChannel(new_channel_name, RequestChannel::SERVER_SIDE);

  // -- Create new thread to handle request channel

  pthread_t thread_id;
  //  cout << "starting new thread " << nthreads << endl;
  if (error = pthread_create(& thread_id, NULL, handle_data_requests, data_channel)) {
    fprintf(stderr, "p_create failed: %s\n", strerror(error));
  }  

}

/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS -- THE PROCESS REQUEST LOOP */
/*--------------------------------------------------------------------------*/

void process_request(RequestChannel & _channel, const string & _request) {

  if (_request.compare(0, 5, "hello") == 0) {
    process_hello(_channel, _request);
  }
  else if (_request.compare(0, 14, "data Joe Smith") == 0) {
    process_data_Joe(_channel, _request);
  }
  else if (_request.compare(0, 15, "data Jane Smith") == 0) {
    process_data_Jane(_channel, _request);
  }
  else if (_request.compare(0, 13, "data John Doe") == 0) {
    process_data_John(_channel, _request);
  }
  else if (_request.compare(0, 9, "newthread") == 0) {
    process_newthread(_channel, _request);
  }
  else {
    _channel.cwrite("unknown request");
  }

}

void* handle_process_loop(RequestChannel & _channel) {

  for(;;) {

    //cout << "Reading next request from channel (" << _channel.name() << ") ..." << flush;
    string request = _channel.cread();
    //cout << " done (" << _channel.name() << ")." << endl;
    //cout << "New request is " << request << endl;

    if (request.compare("quit") == 0) {
      _channel.cwrite("bye");
      usleep(10000);          // give the other end a bit of time.
      break;                  // break out of the loop;
    }

    process_request(_channel, request);
  }
  
}

//we need to pass this as an argument. 
const int  message_buffer_size= 255;
void* handle_request(void* ns){
	//read request
	int* fd= (int*)ns;
	cerr<<"fd:"<<*fd;
	char buf[message_buffer_size];
	for(;;) {

	//read request
		

    if(read(*fd,buf,message_buffer_size)<0)
	    cerr<<"error reading";
	else{
		string request=buf;
		if (request.compare(0, 5, "hello") == 0) {
			string msg="hello to you too";
			const char * s = msg.c_str();
			write(*fd, s,strlen(s)+1);
		}
		else if (request.compare(0, 14, "data Joe Smith") == 0) {
			usleep(1000 + (rand() % 5000));
			string msg=int2string(rand() % 100);
			const char * s = msg.c_str();
			write(*fd, s,strlen(s)+1);
		}
		else if (request.compare(0, 15, "data Jane Smith") == 0) {
			usleep(1000 + (rand() % 5000));
			string msg=int2string(rand() % 100);
			const char * s = msg.c_str();
			write(*fd, s,strlen(s)+1);
		}
		else if (request.compare(0, 13, "data John Doe") == 0) {
			usleep(1000 + (rand() % 5000));
			string msg=int2string(rand() % 100);
			const char * s = msg.c_str();
			write(*fd, s,strlen(s)+1);
		}
		else if(request.compare("quit") == 0){
			
			string msg="bye";
			const char * s = msg.c_str();
			write(*fd, s,strlen(s)+1);
		}
		else {
			string msg="unknow request";
			const char * s = msg.c_str();
			write(*fd, s,strlen(s)+1);
		}
	}
  }

}


/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/
int p,b,c;
int main(int argc, char * argv[]) {
  p = 6002;
  b = 0;
	while ((c = getopt (argc,argv,"p:b")) != -1)
	{
		switch(c)
		{
		  case 'p': //port number of server host
				p = atoi(optarg);
				if(p == 0) 
				{
					p = 1212;
				}
				break;
			case 'b': /// size of bounded buffer
				b = atoi(optarg);
				if(b == 0)
				{
					b = 100;
				}
				break;
			case '?':
				 if (optopt == 'c')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint (optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
				return 1;
			default:
				abort();
				
		}
	}



  //  cout << "Establishing control channel... " << flush;
  //this format may be incorrect
  NetworkRequestChannel* rc = new NetworkRequestChannel(p,b, handle_request);
  //  cout << "done.\n" << flush;

  

}
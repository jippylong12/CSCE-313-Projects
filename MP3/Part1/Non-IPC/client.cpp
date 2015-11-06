/* 
    File: simpleclient.cpp

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 2012/07/11

    Simple client main program for MP2 in CSCE 313
*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cassert>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <math.h>

#include <vector>
#include <errno.h>
#include <unistd.h>

#include "reqchannel.h"

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
/* LOCAL FUNCTIONS -- SUPPORT FUNCTIONS */
/*--------------------------------------------------------------------------*/

string int2string(int number) {
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}


string local_send_request()
{
	
	return int2string(rand() % 100);
}

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
    
    /* -- Start sending a sequence of requests */
    string reply1 = chan.send_request("hello");
    cout << "Reply to request 'hello' is '" << reply1 << "'" << endl;
    
    string reply2 = chan.send_request("data Joe Smith");
    cout << "Reply to request 'data Joe Smith' is '" << reply2 << "'" << endl;
    
    string reply3 = chan.send_request("data Jane Smith");
    cout << "Reply to request 'data Jane Smith' is '" << reply3 << "'" << endl;
    
    double mean;
    vector<double> hold;
    for(int i = 0; i < 100000; i++) 
    {
        string request_string("data TestPerson" + int2string(i));
        clock_t t1;
        t1 = clock();
        //string reply_string = chan.send_request(request_string);
		string reply_string = local_send_request();
		cout << "reply to request " << i << ":" << reply_string<< endl;
        t1 = clock() - t1;
        double t = ((double)t1)/CLOCKS_PER_SEC;
        hold.push_back(t);
        mean += t;
        if(i == 99999) 
        {
            mean = mean/i;
        }
    }
    double deviation = 0;
    for(int i = 0;i<100000; i++)
    {
        deviation += (hold[i] - mean)*(hold[i]-mean);
        if(i == 99999)
        {
            deviation = deviation/(i+1);
            sqrt(deviation);
        }
    }
    
    mean = mean * 1000000;
    deviation = deviation *1000000;
    cout<<"The average is "<<mean<<" microseconds."<<endl;
    cout<<"The standard deviation is "<<deviation<<" microseconds."<<endl;
    
    string reply4 = chan.send_request("quit");
    cout << "Reply to request 'quit' is '" << reply4 << endl;
    
    usleep(1000000);
}

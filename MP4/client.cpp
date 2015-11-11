
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
#include <vector>
#include <ctime>

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
struct Histogram{ //class that holds the people's responses
	vector<int> john;
	vector<int> jane;
	vector<int> joe;
	Histogram(){}
};
int n,b,w;
Histogram data=Histogram();
void* RT(void* arg){ //request thread
	int person=*(int *)arg;
	if(person==0){//Joe
		for(int i=0;i<n;i++)
			request_buffer.push("data Joe Smith"); //pushes requests into to request buffers
		
	}
	
	else if(person==1){//Jane
		for(int i=0;i<n;i++)
			request_buffer.push("data Jane Smith");
	}
	
	else if (person==2){//John
		for(int i=0;i<n;i++)
			request_buffer.push("data John Doe");
	}

	
	return NULL;
		//cerr<<"Something went wrong request buffer";
	
}


void* WT(void* arg){ //worker thread
	RequestChannel channel_req=*(RequestChannel *)arg;
	string stringJoe = "data Joe Smith";
	string stringJane = "data Jane Smith";
	string stringJohn = "data John Doe";
	string stringStop = "#";
	while(true){
		string nq=request_buffer.pop(); // pulls request from buffer
		if(nq.compare(stringStop) == 0)
		{
			request_buffer.push(nq);
			string resp=channel_req.send_request("quit");
			delete (RequestChannel *)arg;
			return NULL;
		}
		string resp=channel_req.send_request(nq); //send request to server and got a response back

		if(nq.compare(stringJohn)  == 0)
		{
			response_buffer[2].push(resp); //push response into their right response buffers
			
		}
		else if(nq.compare(stringJoe) == 0)
		{
			response_buffer[0].push(resp);
			
		}
		else if(nq.compare(stringJane) == 0)
		{
			response_buffer[1].push(resp);
			
		}
		else
		{
			cerr<<"something went wrong in the worker thread "<<nq<<endl;
		}
	}
	
}

void* ST(void* arg){ //stat thread
	int person=*(int *)arg;
	cout<<"person "<<person<<endl;
	if(person==0){//Joe
		for(int i = 0; i<n; ++i)
		{
			int temp=stoi(response_buffer[0].pop());
			data.joe.push_back(temp);
			
		}
	}
	
	else if(person==1){//Jane
		for(int i = 0; i<n; ++i)
		{
			int temp=stoi(response_buffer[1].pop());
			data.jane.push_back(temp);
			
		}
	}
	
	else if (person==2){//John
		for(int i = 0; i<n; ++i)
		{	
			int temp=stoi(response_buffer[2].pop());
			data.john.push_back(temp);
		}
	}
	
	return NULL;
}

int main(int argc, char * argv[]) {
	clock_t begin= clock();
	clock_t done= clock();
	int c;
	n=10000;
	b=100;
	w=10;
	while ((c = getopt (argc,argv,"n:b:w:")) != -1)
	{
		switch(c)
		{
			case 'n':
				n = atoi(optarg);
				if( n == 0)
				{
					n = 1000;
				}
				break;
			case 'b':
				b = atoi(optarg);
				if(b == 0)
				{
					b = 100;
				}
				break;
			case 'w':
				w = atoi(optarg);
				if(w == 0)
				{
					w = 10;
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
	clock_t start,end;
	start=clock(); // start the clock
    pid_t pid = fork();
    //if child process
    //we need to start up the data server
    if(pid == 0) 
    {
        execvp("./dataserver",argv);
    }
	else
	{
		
		int size=b;
		request_buffer= BB(size);
		for(int i=0;i<3;i++)
			response_buffer[i]= BB(size/3);
		
		
		
		cout << "CLIENT STARTED:" << endl;

		cout << "Establishing control channel... " << flush;
		RequestChannel chan("control", RequestChannel::CLIENT_SIDE);
		cout << "done." << endl;
		
		pthread_t requestid[3];
		pthread_t workerid[w];
		pthread_t responseid[3];
		
		/* -- Start sending a sequence of requests */
		for (int i=0; i<3;i++)
		{
			pthread_create (&requestid[i],0, RT, new int(i)); // create three request threads
			
		}
		vector<RequestChannel*> chan1;
		for(int i=0;i<w;i++)
		{
			string channel_name=chan.send_request("newthread");
			chan1.push_back(new RequestChannel(channel_name,RequestChannel::CLIENT_SIDE)); //creates all the channels for the worker threads
		}
		for(int i=0;i<w;i++)
		{
			pthread_create(&workerid[i], 0, WT , chan1[i]);// create 3 worker threads
		}
		//response_buffer[0].isempty();
	 	 for (int i=0; i<3;i++)
		{
			pthread_create (&responseid[i],0, ST,new int(i)); // create 3 stat threads
			
		} 
		
		for(int i=0; i<3 ; i++)
		{
			pthread_join(requestid[i],NULL);
		}
		request_buffer.push("#"); //tells when to stop worker threads
		
		for(int i=0; i<w ; i++)
		{
			pthread_join(workerid[i],NULL);
		}
		for(int i=0; i<3 ; i++)
		{
			pthread_join(responseid[i],NULL);
		}
		end=clock(); // end the clock
		cout<<"N"<<n<<endl;
		cout<<"B"<<b<<endl;
		cout<<"W"<<w<<endl;
		cout<<"JOE's:"<<data.joe.size()<<endl;
		/* for(int a:data.joe)
			cout<<a<<endl; */
		cout<<"JANE's:"<<data.jane.size()<<endl;
		/* for(int a:data.jane)
			cout<<a<<endl; */
		cout<<"JOHN's:"<<data.john.size()<<endl;
		/* for(int a:data.john)
			cout<<a<<endl; */
		string reply4 = chan.send_request("quit");
		cout << "Reply to request 'quit' is '" << reply4 << "'" << endl;
		
		usleep(1000000);
		double time=(double)(end-start)/CLOCKS_PER_SEC;
		cout<<" The perfomance time took " << time<<endl;
		}
}

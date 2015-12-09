
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
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/select.h>
#include <fstream>
#include <algorithm>
#include <signal.h>

#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <thread>
#include <vector>
#include <ctime>

#include "reqchannel.h"
#include "NetworkRequestChannel.h"
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

int n,b,w,p;
string h;
//we need an array of request channel pointers in order for us to construct it in the main. 
vector<NetworkRequestChannel*> NRC; //the size is determined by the input w of the program and done in the main


//boundary buffer for the request threads
BB request_buffer;
//we need three boundary buffers for each person's response. 
vector<BB> response_buffer(3);
struct Histogram{ //class that holds the people's responses
	vector<int> john;
	vector<int> jane;
	vector<int> joe;
	Histogram(){}
	void show_histogram(int i,ofstream& file);
	void show_current();
};
void Histogram:: show_current(){
	int count=0;
	int tenth=0;
	//john
	cout<<"JOHN"<<endl;
		for(int i=0;i<100;i++){
			tenth++;
			for(int a=0;a<john.size();a++){
				if(john[a]==i)
				{
					count++;
				}	
			}
			if(tenth==10){
				tenth=0;
				cout<<i-9<<"-"<<i<<":"<<count<<endl;
				count=0;
			}		
		}
		cout<<"total size so far :"<< john.size()<<endl<<endl;
	//jane
	cout<<"JANE"<<endl;
	count=0;
	tenth=0;
		for(int i=0;i<100;i++){
			tenth++;
			for(int a=0;a<jane.size();a++){
				if(jane[a]==i)
				{
					count++;
				}	
			}
			if(tenth==10){
				tenth=0;
				cout<<i-9<<"-"<<i<<":"<<count<<endl;
				count=0;
			}
			
		}
		cout<<"total size so far :"<< jane.size()<<endl<<endl;
	//joe
	count=0;
	tenth=0;
	cout<<"JOE"<<endl;
		for(int i=0;i<100;i++){
			tenth++;
			for(int a=0;a<joe.size();a++){
				if(joe[a]==i)
				{
					count++;
				}	
			}
			if(tenth==10){
				tenth=0;
				cout<<i-9<<"-"<<i<<":"<<count<<endl;
				count=0;
			}
			
		}
		cout<<"total size so far :"<< joe.size()<<endl<<"--------------------------"<<endl<<endl<<endl<<endl;
}
void Histogram:: show_histogram(int i, ofstream& file){ // frequency of numbers between 0 to 99
	int count=0;
	int tenth=0;
	if(i==0){//john
		for(int i=0;i<100;i++){
			int countFile = 0;
			tenth++;
			for(int a:john){
				if(a==i)
				{
					count++;
					countFile++;
				}	
			}
			if(tenth==10){
				tenth=0;
				cout<<i-9<<"-"<<i<<":"<<count<<endl;
				file<<count<<endl;
				count=0;
			}
			if(tenth==10){
				tenth=0;
				cout<<i-9<<"-"<<i<<":"<<count<<endl;
				count=0;
			}			
		}
	}
	else if(i==1){//jane
		for(int i=0;i<100;i++){
			int countFile = 0;
			tenth++;
			for(int a:jane){
				if(a==i)
				{
					count++;
					countFile++;
				}	
			}
			if(tenth==10){
				tenth=0;
				cout<<i-9<<"-"<<i<<":"<<count<<endl;
				file<<count<<endl;
				count=0;
			}
			
		}
	}
	else if(i==2){//joe
		for(int i=0;i<100;i++){
			int countFile = 0;
			tenth++;
			for(int a:joe){
				if(a==i)
				{
					count++;
					countFile++;
				}	
			}
			if(tenth==10){
				tenth=0;
				cout<<i-9<<"-"<<i<<":"<<count<<endl;
				file<<count<<endl;
				count=0;
			}
			
		}
	}
}

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


void* EHT(void* arg)//event handler thread
{ 
	
	fd_set s; // create the set
	fd_set sBackup; //to restore the set
	string stringJoe = "data Joe Smith";
	string stringJane = "data Jane Smith";
	string stringJohn = "data John Doe";
	int maxfd = 0; //for the largest file descriptor value
	vector<string> response_input; //keeps track of who the responses belong to
	response_input.resize(w);
	int num_responses=0;
	
	FD_ZERO(&s); // zero out the set
	while(num_responses<(n*3))
	{
		cerr<<"in EHT";
		s = sBackup; //restore the backup
		int counterForThreads = 0; //knowing when to exit last for loop
		//we need to fill out the set everytime since select destroys it
		
		for(int i = 0; i<w-1;++i) //fill the set and send the requests
		{
			cerr<<"in EHT1";
			if(FD_ISSET(NRC[i]->read_fd(),&s) != 1) //if the file descriptor is not in the set already
			{	
				cerr<<"in EHT2";
				FD_SET(NRC[i]-> read_fd(),&s); //fill set with each request channel file descriptor
				if(NRC[i] -> read_fd() > maxfd) //if the file descriptor we are at is bigger than the max
				{
					maxfd = NRC[i]->read_fd(); // make it the new max
					cerr<<"in EHT3";
				}
			if(request_buffer.size() < 1) continue; // we should go to the next iteration if we have nothing in the buffer
			string send_request=request_buffer.pop();
			NRC[i] -> cwrite(send_request); //give the request channel something to send to the server
			response_input[i]=send_request;
			// keep count of response plus the connection request
			}
			
		}
		cerr<<"in EHT4";
		sBackup = s; // back up the set
		//then we will periodically check to see if a thread is done and then read that. 
		int k = 0; //how many threads are ready to be checked. 
		while(k == 0) //while none of the threads are ready. 
		{
			cerr<<"in EHT5";
			k=select(maxfd+1,&s,NULL,NULL,NULL); //check to see if there is a thread ready
			//Once it breaks the set is destroyed except for the ready threads
			cerr<<"in EHT66";

		}

		//when we get to this point we should have a thread ready and need
		//to get the response. 
		for(int i=0; i<w; i++) //check through each request channel
		{
			cerr<<"in EHT6";
			if(counterForThreads == k) break; // we have found all the threads possible so there is no need to keep searching
			if(FD_ISSET(NRC[i]->read_fd(),&s)) //if we find the file descriptor in this request channel in the set
			{
				++counterForThreads; //increase the count that is keeping track of how many we have worked on
				string returnString = NRC[i]-> cread(); // get the processed data 
				num_responses++;
				//git cout<<num_responses<<endl;
				FD_CLR(NRC[i]->read_fd(),&sBackup); //remove the file descriptor from the set because we used it
				//This where we put returnString into a response buffer **need a way to know which response buffer to put it in. **
				
				if(response_input[i].compare(stringJoe) == 0)
				{
					response_buffer[0].push(returnString);
					
				}
				else if(response_input[i].compare(stringJane) == 0)
				{
					response_buffer[1].push(returnString);
					
				}
				else if(response_input[i].compare(stringJohn)  == 0)
				{
					response_buffer[2].push(returnString); //push response into their right response buffers
					
				}
				else
				{
					cerr<<"something went wrong in the worker thread "<<response_input[i]<<endl;
				}
						
				
				/* if(request_buffer.size() < 1) continue; // we should go to the next iteration if we have nothing in the buffer
				// do not over pop request buffer bc you will get stuck
				string send_request=request_buffer.pop();
				RC[i]->cwrite(send_request); //send another request to data server
				response_input[i]=send_request; */
				
			}
		}
	}
}

void* ST(void* arg){ //stat thread
	int person=*(int *)arg;
	//cout<<"person "<<person<<endl;
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

void catch_alarm(int sig){
	data.show_current();
	signal(sig,catch_alarm);
}
//clock function for timing. 
//I'd like to thank Jason on stackoverflow for making this custom clock for multithreaded programs
//it's the only thing in the program that isn't ours. It's just for analysis. 
double my_clock(void) {
  struct timeval t;
  gettimeofday(&t, NULL);
  return (1.0e-6*t.tv_usec + t.tv_sec);
}

int main(int argc, char * argv[]) {
	/* struct sigaction periodic_histogram;
	periodic_histogram.sa_handler=catch_alarm;
	periodic_histogram.sa_flags=SA_RESTART; // deals with EINTR errors
	struct itimerval timer={0};
	timer.it_value.tv_sec=1;
	timer.it_interval.tv_sec=1;
	sigaction(SIGALRM,&periodic_histogram,NULL);
	setitimer(ITIMER_REAL,&timer,NULL); */
	int c;
	n=100;
	b=100;
	w=10;
	h = "linux2";
	p = 1515;
	while ((c = getopt (argc,argv,"n:b:w:h:p")) != -1)
	{
		switch(c)
		{
			case 'n': //number of data requests per person
				n = atoi(optarg);
				if( n == 0)
				{
					n = 1000;
				}
				break;
			case 'b': /// size of bounded buffer
				b = atoi(optarg);
				if(b == 0)
				{
					b = 100;
				}
				break;
			case 'w': //number of request channels
				w = atoi(optarg);
				if(w == 0)
				{
					w = 10;
				}
				break;
			case 'h': //name of server host
				if(h == "") // we don't need atoi since it is a string
				{
					h = "linux2";
				}
				break;
			case 'p': //port number of server host
				p = atoi(optarg);
				if(p == 0) 
				{
					p = 1212;
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
	// clock_t start,end;
	 double start, end;
	start = my_clock();
	// start=clock(); // start the clock
	
	
	
	
	int size=b;
	request_buffer= BB(size);
	for(int i=0;i<3;i++)
		response_buffer[i]= BB(size/3);
	
	
	cout << "CLIENT STARTED:" << endl;

	cout << "Establishing control channel... " << flush;
	
	
	pthread_t requestid[3];
	pthread_t eventHandlerid;
	pthread_t responseid[3];
	
	// Start sending a sequence of requests
	for (int i=0; i<3;i++)
	{
		pthread_create (&requestid[i],0, RT, new int(i)); // create three request threads
		
	}
	
	// create the new Network Request channels
	for(int i=0;i<w;i++)
	{
		NRC.push_back(new NetworkRequestChannel(h,p)); //client
	}
	
	//create the thread for the eventHandler
	pthread_create(&eventHandlerid,0,EHT,0);
	
	//response_buffer[0].isempty();
 	 for (int i=0; i<3;i++)
	{
		pthread_create (&responseid[i],0, ST,new int(i)); // create 3 stat threads
	} 
	
	for(int i=0; i<3 ; i++)
	{
		pthread_join(requestid[i],NULL);
	}
	pthread_join(eventHandlerid,NULL);
	//request_buffer.push("#"); //tells when to stop worker threads
	for(int i=0; i<3 ; i++)
	{
		pthread_join(responseid[i],NULL);
	}
	
	//joining the event handler thread **The EH thread does not exit currently**
	
	
	
	//end=clock(); // end the clock
	end = my_clock();
	cout<<"N"<<n<<endl;
	cout<<"B"<<b<<endl;
	cout<<"W"<<w<<endl;
	for(int i=0;i<NRC.size();i++){
		NRC[i]->send_request("quit");
		delete NRC[i];
	}
	/* string reply4 = chan.send_request("quit");
	cout << "Reply to request 'quit' is '" << reply4 << "'" << endl; */
	
	usleep(1000000);
	
	//double time=((double)(end-start))/CLOCKS_PER_SEC;
	double time = end-start;
	cout<<"\n \n \n The perfomance time took " << time<<" seconds."<<endl;
	cout<<"Creating data files for histogram"<<endl; //start outputting data to files
	//create streams
	ofstream joeData;
	ofstream janeData;
	ofstream johnData;
	/*
	//open files for writing
	joeData.open("joeData.txt");
	janeData.open("janeData.txt");
	johnData.open("johnData.txt");
	
	cout<<"JOE's:"<<data.joe.size()<<endl;
	data.show_histogram(2,joeData);
	for(int a:data.joe)
		cout<<a<<endl;
	cout<<"JANE's:"<<data.jane.size()<<endl;
	data.show_histogram(1,janeData);
	for(int a:data.jane)
		cout<<a<<endl;
	cout<<"JOHN's:"<<data.john.size()<<endl;
	data.show_histogram(0,johnData);
	for(int a:data.john)
		cout<<a<<endl;
	
	//close files. 
	joeData.close();
	janeData.close();
	johnData.close(); */

		
}

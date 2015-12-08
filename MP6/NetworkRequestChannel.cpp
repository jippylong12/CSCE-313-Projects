#include "NetworkRequestChannel.h"



using namespace std;

NetworkRequestChannel::NetworkRequestChannel(const string _server_host_name, const unsigned short _port_no); //client
{
    //these should be set up like the files they gave  us for the TDP
}


NetworkRequestChannel::NetworkRequestChannel(const unsigned short _port_no, int backlog,  void * (*connection_handler) (int *)) //server
{
    //these should be set up like the files they gave  us for the TDP
    //socket
    //bind
    //listen
    /*while()
    {
        newsocket = accept()
        NetworkRequestChannel* new NRC(newsocket)
        pthread_create(connection_handler,newRC)
    }*/
}

NetworkRequestChannel::~NetworkRequestChannel()
{
    
}

string NetworkRequestChannel::send_request(string _request)
{
    
}

string NetworkRequestChannel::cread()
{
    //read(socket, )
}

int NetworkRequestChannel::cwrite(string _msg)
{
    //wriet(socket, functions?)
}
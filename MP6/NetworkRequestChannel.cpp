#include "NetworkRequestChannel.h"
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

NetworkRequestChannel::NetworkRequestChannel(const string _server_host_name, const unsigned short _port_no) //client
{
	struct addrinfo hints, *res;
	int sockfd;
	string temp_socket=_server_host_name;
	const char* socket_name=temp_socket.c_str();
	stringstream ss;
	ss << _port_no;
	const char* port_num= (ss.str()).c_str();
	// first, load up address structs with getaddrinfo():

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	int status;
	//getaddrinfo("www.example.com", "3490", &hints, &res);
	if ((status = getaddrinfo(socket_name, port_num, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        //return -1;
		exit(1);
    }

	// make a socket:
	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sockfd < 0)
	{
		perror ("Error creating socket\n");	
		exit(1);
		//return -1;
	}

	// connect!
	if (connect(sockfd, res->ai_addr, res->ai_addrlen)<0)
	{
		perror ("connect error\n");
		//return -1;
		exit(1);
	}
	else{
	fd=sockfd;
	
	printf ("Successfully connected to the server %s\n", socket_name);
	printf ("Now Attempting to send a message to the server\n", socket_name);
	/* char buf [message_buffer_size];
	sprintf (buf, "hello");
	write (sockfd, buf, strlen (buf)+1);
	read (sockfd, buf, message_buffer_size);
	printf ("Received %s from the server\n", buf);
	write (sockfd, buf, strlen (buf)+1);
	read (sockfd, buf, message_buffer_size);
	printf ("Received %s from the server\n", buf); */
	}
}


NetworkRequestChannel::NetworkRequestChannel(const unsigned short _port_no, int backlog,  void * (*connection_handler) (void *)) //server
{

	int sockfd;  // listen on sock_fd, new connection on new_fd
	int* new_fd;
    struct addrinfo hints, *serv;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    char s[INET6_ADDRSTRLEN];
    int rv;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP
	stringstream ss;
	ss << _port_no;
	const char* port_num= (ss.str()).c_str();
    if ((rv = getaddrinfo(NULL, port_num, &hints, &serv)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        //return -1;
    }
	if ((sockfd = socket(serv->ai_family, serv->ai_socktype, serv->ai_protocol)) == -1) {
        perror("server: socket");
		//return -1;
    }
    if (bind(sockfd, serv->ai_addr, serv->ai_addrlen) == -1) {
		close(sockfd);
		perror("server: bind");
		//return -1;
	}
    freeaddrinfo(serv); // all done with this structure

    if (listen(sockfd, 20) == -1) {
        perror("listen");
        exit(1);
    }
	
	printf("server: waiting for connections...\n");
	char buf [255];
	pthread_t server;
	while(1) 
	{  // main accept() loop
        sin_size = sizeof their_addr;
		int fds;
        fds = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		*new_fd= fds;
		if(fds>0){
		cerr<<fds;
		cerr<<*new_fd;
		}
        if (*new_fd <0)
            cerr<<("accept");
		else{
			/* char buf[message_buffer_size];
			
			read(*new_fd,&buf,message_buffer_size);
			cerr<<("reading");
			string request=buf;
			string msg="hello to you too";
			const char * s = msg.c_str();
			write(*new_fd, s,strlen(s)+1);
			cerr<<("writing");  */
			pthread_create (&server,0, connection_handler, new_fd);
		}
			
			/* delete new_fd;
			new_fd=new int(); */
    }
}

NetworkRequestChannel::~NetworkRequestChannel()
{
    close(fd);
}

string NetworkRequestChannel::send_request(string _request)
{
    cwrite(_request);
	string s = cread();
	return s;
}

string NetworkRequestChannel::cread()
{
	
	char buf[message_buffer_size];

	if (read(fd, buf, message_buffer_size) < 0)
	{
		perror(string("Request Channel (" + to_string(fd) + "): Error reading from socket!").c_str());
	}
  
	string s = buf;

	return s;
}

int NetworkRequestChannel::cwrite(string _msg)
{
	if (_msg.length() >= message_buffer_size)
	{
		cerr << "Message too long for Channel!\n";
		return -1;
	}

	const char * s = _msg.c_str();

	if (write(fd, s, strlen(s)+1) < 0)
	{
		perror(string("Request Channel (" + to_string(fd)+ ") : Error writing to socket!").c_str());
	}
}

int NetworkRequestChannel::read_fd()
{
	return fd;
}

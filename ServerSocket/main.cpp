// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <pthread.h>
//#define PORT 8080
#define SUCCESS 0
#define FAIL 1

using namespace std;

string response[6] = {"Zero","one","two","three","four","five"};

void* ProcessClientRequest(void* ) ;

int main(int argc, char const *argv[])
{

	if(argc != 2 )
	{
		cout << "Usage : ./Server port_num" << endl;
		return 1;
	}
	else
	{
		cout << "Server is created at port no. " << argv[1] << endl;
	}

	pthread_t threads;
	int PORT = atoi(argv[1]);

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
      
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    cout << "Waiting for request from client " << endl;

    while(true)
    {
    	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                   (socklen_t*)&addrlen))<0)
	    {
	        perror("accept");
	        exit(EXIT_FAILURE);
	    }

	     cout << "new request accepted at socket id " << new_socket <<  endl;


	    int rc = pthread_create(&threads, NULL, ProcessClientRequest, (void*)new_socket);

	    if(rc) 
	    {
	         cout << "Error:unable to create thread," << rc << endl;
	         exit(-1);
	    }
	    else
	    	cout <<"Thread created " << endl;

	}

    pthread_join(threads,NULL);

    //ProcessClientRequest(new_socket);

    /*while(true)
    {
    	
	    valread = read( new_socket , buffer, 1024);
	    //printf("%s\n",buffer );

	    int pos = buffer[0] - 48;

	    cout << endl << "Received ... " << pos << " from client " << endl;

	    if(buffer[0] == '#')
	    {
	    	cout << "exiting server ..." << endl;
	    	break;
	    }
	    send(new_socket , response[pos].c_str() , response[pos].length() , 0 );
	    
	    cout << endl << "Sending ... " << response[pos] <<" to client" << endl;
	    //close(new_socket);
    }*/
    
    return 0;
}

void* ProcessClientRequest(void* isock_id)
{
	int sock_id = (int)isock_id;
	char buffer[1024] = {0};
	int valread ;

	cout << "sock_id " << sock_id << endl;
	 while(true)
    {
    	
	    valread = read( sock_id , buffer, 1024);
	    //printf("%s\n",buffer );

	    cout << "No of bytes received " << valread << endl;
	    if(valread <= 0 )
	    	continue;

	    int pos = 0;
	    if( (buffer[0] - 48) < 6 )
	     	pos = buffer[0] - 48;

	    cout << endl << "Received ... " << pos << " from client " << endl;

	    if(buffer[0] == '#')
	    {
	    	cout << "exiting server ..." << endl;
	    	break;
	    }
	    send(sock_id , response[pos].c_str() , response[pos].length() , 0 );
	    
	    cout << endl << "Sending ... " << response[pos] <<" to client" << endl;
    }
    //close(sock_id);
	//return NULL ;
}
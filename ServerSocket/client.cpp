// Client side C/C++ program to demonstrate Socket programming

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>

using namespace std;
//#define PORT 8080
  
int main(int argc, char const *argv[])
{

    if(argc != 3 )
    {

        cout << "Usage ./client hostname port_no." << endl;
        return 1;
    }
    
    int PORT = atoi(argv[2]);

    string hostname = argv[1];
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    //char *hello = "Hello from client";

    char DataToSent[2] = {'\0'};
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, hostname.c_str(), &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    int i = 0;
    while( i < 6 )
    {
        memset(DataToSent,'\0',sizeof(DataToSent));

        cout << "Enter number " << endl;

        int j ;
        cin >> j ;
        sprintf(DataToSent,"%d",j);

        cout << "Sending ... " << DataToSent << " to server " << endl; 
        send(sock , DataToSent , strlen(DataToSent) , 0 );
        
        valread = read( sock , buffer, 1024);

        //cout << "Receiving ... "<< buffer < " from server " << endl;
        printf("%s\n",buffer );
        i++;
    }

    send(sock , "#" , strlen(DataToSent) , 0 );
    close(sock);

    return 0;

}

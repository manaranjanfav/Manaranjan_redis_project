
// author Manaranjan Saren

// Client side C++ programme for redis implementation

#include<bits/stdc++.h>
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
using namespace std;
#define MAX 1025
int main(int argc, char const *argv[]) 
{ 

	if(1>=argc)
    {
    	  cout<<"Erro!!!\n";
    	  cout<<"Please provide the PORT number\n";
    	  return 0;
    }

	int PORT;

    string port=argv[1];
    PORT=stoi(port);

	int sock = 0, valread; 
	int ret=-1;
	struct sockaddr_in serv_addr; 
	char hello[1025];
	char message[1025];
	char buffer[1025];
	char leave[1025]="exit\n";
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		return -1; 
	} 

	valread = read( sock , message, 1025); 
	printf("%s\n",message ); 
    
	while(1)
	{

     cout<<"redis> ";
	 memset(hello, 0, 1025);
     fgets(hello, MAX, stdin);
     ret=strcmp(hello,leave);
     
     if(ret==0)
     	break;
	send(sock , hello , strlen(hello) , 0 ); 
	//printf("Hello message sent\n"); 
	//printf("%s\n",buffer );
	memset(buffer, 0, 1025);
	valread = read( sock , buffer, 1025); 
	cout<<"redis> ";
	printf("%s",buffer ); 

   }
	return 0; 
} 

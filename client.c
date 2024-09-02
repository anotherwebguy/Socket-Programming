/*
	filename server_ipaddress portno
*/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<netinet/in.h>
#include<netdb.h>
#include<unistd.h>

void error(const char* message)
{
	perror(message);
	exit(1);
}

int main(int argc, char* argv[])
{
	if(argc < 3)
	{
		fprintf(stderr,"usage %s hostname port\n",argv[0]);
		exit(1);
	}
	int sockfd, newsockfd, portno, n;
	struct sockaddr_in server_addr;
	struct hostent *server;
	
	char buffer[1024];
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0)
	{
		error("ERROR opening socket");
	}
	server = gethostbyname(argv[1]);
	if(server == NULL)
	{
		error("Error, no such host");
	}
	memset(&server_addr, 0, sizeof(server_addr));
	
	server_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char *) &server_addr.sin_addr.s_addr, server->h_length);
	server_addr.sin_port = htons(portno);
	
	if(connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
	{
		error("Connection failed");
	}
	while(1)
	{
		bzero(buffer, 1024);
		printf("You: ");
		fgets(buffer, 1024, stdin);
		
		size_t len = strlen(buffer);
		if (len > 0 && buffer[len-1] == '\n')
		    buffer[len-1] = '\0';
		
		n = write(sockfd, buffer, strlen(buffer));
		if(n < 0) 
		{
			error("Error on writing");
		}
		
		bzero(buffer, 1024);
		n = read(sockfd, buffer, 1024);
		if(n < 0) 
		{
			error("Error on reading");
		}
		printf("Server : %s\n",buffer);
		
		int i = strncmp("Bye",buffer,3);
		if(i == 0) 
		{
			break;
		}
	}
	close(sockfd);
	return 0;	
}



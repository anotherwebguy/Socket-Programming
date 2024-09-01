#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<netinet/in.h>
#include <unistd.h>

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		fprintf(stderr,"Port No no provided. Program termnated\n");
		exit(1);
	}
	
	int sockfd, newsockfd, portno, n;
	char buffer[1024];
	struct sockaddr_in server_addr, client_addr;
	socklen_t clientlen;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd<0)
	{
		error("Error opening Socket.");
	}
	memset(&server_addr, 0, sizeof(server_addr));
	portno = atoi(argv[1]);
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(portno);
	
	if(bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
	{
		error("Binding failed.");
	}
	listen(sockfd, 5);
	clientlen = sizeof(client_addr);
	
	newsockfd = accept(sockfd, (struct sockaddr*) &client_addr, &clientlen);
	
	if(newsockfd < 0)
	{
		error("Error on Accept");
	}
	
	while(1)
	{
		bzero(buffer, 1024-1);
		n = read(newsockfd, buffer, 1024-1);
		if(n < 0) 
		{
			error("Error on reading");
		}
		printf("Client : %s",buffer);
		bzero(buffer, 1024-1);
		fgets(buffer, 1024-1, stdin);
		
		n = write(newsockfd, buffer, sizeof(buffer));
		if(n < 0) 
		{
			error("Error on reading");
		}
		int i = strncmp("Bye",buffer,3);
		if(i == 0) 
		{
			break;
		}
	}
	close(newsockfd);
	close(sockfd);
	return 0;	
}

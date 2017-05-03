#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h> 
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <signal.h>
#include <fcntl.h>


#define MAX_MESSAGE 500
#define MAX_HANDLE 10 

void initiateContact() {

}

void sendMessage() {

}

void receiveMessage() {

}

int main(int argc, char *argv[]) {
	char *hostname, *port;
	char handle[MAX_HANDLE]; 
	char message[MAX_MESSAGE];

	int socketfd; 

	//check args
	if (argc < 3) { fprintf(stderr,"USAGE: ./chatclient <server-hostname> <port #>\n"); exit(1); } 
	hostname = argv[1];
	port = argv[2];

	printf("hostname: %s, port: %s\n", hostname, port);

	/*
	** fill out hints struct 
	*/
	struct addrinfo hints, *res;
	int status; 
	char ipstr[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof hints);
	//hints.ai_family = AF_UNSPEC; 			//IPv4 or IPv6
	hints.ai_family = AF_INET; 	
	hints.ai_socktype = SOCK_STREAM;		//TCP
	hints.ai_flags = AI_PASSIVE; 

	//get ready to connect 
	if ((status = getaddrinfo(hostname, port, &hints, &res)) != 0) {
		fprintf(stderr,"getaddrinfo: %s\n", gai_strerror(status)); exit(1); 	
	}

	//freeaddrinfo(res);						//free linked list

	/*
	** make socket and connect 
	*/
	socketfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (socketfd == -1) { fprintf(stderr,"socket\n"); exit(1); }

	status = connect(socketfd, res->ai_addr, res->ai_addrlen);
	if (status == -1) {
		fprintf(stderr,"connect!!\n"); 
		close(socketfd);
		exit(1); 
	}


	/*
	** get handle from user 
	*/
	printf("Enter handle: ");
	fflush(stdout);	fflush(stdin);
	fgets(handle, MAX_HANDLE, stdin);

	//ERROR HANDLING HERE 

	//trim newline 
	handle[strcspn(handle, "\n")] = 0;


	/*
	** run chat 
	*/
	while(1) {
		printf("%s> ", handle);	

		//get message from user 
		memset(message, 0, MAX_MESSAGE);
		fflush(stdout);	fflush(stdin);
		fgets(message, MAX_MESSAGE - 1, stdin);		//account for newline in size

		//trim newline 
		message[strcspn(message, "\n")] = 0;

		printf("%s\n", message);	
	}

	
	close(socketfd);

	return 0;
}
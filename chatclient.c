/*
** Morgan Brenner
** brennemo@oregonstate.edu 
** CS 372 Program 1 
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h> 
#include <netinet/in.h>

#define MAX_MESSAGE 500
#define MAX_HANDLE 10 

/*
**
** Establish connection with server 
*/
int initiateContact(char *hostname, char *port, int status) {
	struct addrinfo hints, *res;
	char ipstr[INET_ADDRSTRLEN];
	
	int socketfd; 
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET; 	
	hints.ai_socktype = SOCK_STREAM;		//TCP
	hints.ai_flags = AI_PASSIVE; 
	
	if ((status = getaddrinfo(hostname, port, &hints, &res)) != 0) {
		fprintf(stderr,"error: getaddrinfo: %s\n", gai_strerror(status)); exit(1); 	
	}
	
	socketfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (socketfd == -1) { fprintf(stderr,"error: socket\n"); exit(1); }

	
	status = connect(socketfd, res->ai_addr, res->ai_addrlen);
	if (status == -1) {
		fprintf(stderr,"error: connect\n"); 
		close(socketfd);
		exit(1); 
	}
	
		
	//freeaddrinfo(res);						//free linked list
	
	return socketfd;
	
}

/*
**
** Retrieve and store handles for both client and server users 
*/
void exchangeHandles(int socketfd, char *handleA, char *handleB) {
	int charsWritten, charsRead;
	
	//get handle from user 
	memset(handleB, 0, MAX_HANDLE + 2);
	printf("Enter handle: ");
	fflush(stdout);	fflush(stdin);
	fgets(handleB, MAX_HANDLE + 2, stdin);
	handleB[strcspn(handleB, "\n")] = 0;			//trim newline 	
	
	//send handle to server 
	charsWritten = send(socketfd, handleB, strlen(handleB), 0);
	if (charsWritten < 0) { fprintf(stderr,"error: send handle\n"); exit(1); };
	
	//get handle from server 
	memset(handleA, 0, MAX_HANDLE + 2);
	charsRead = recv(socketfd, handleA, MAX_HANDLE + 1, 0);
	if (charsRead < 0) { fprintf(stderr,"error: receive handle\n"); exit(1); };	
}


/*
** 
** Returns 1 to quit chat, 0 to continue 
*/
int sendMessage(int socketfd, char *message, char *handleB) {
	int charsWritten;
	char *quitChat = "\\quit";
	
	//get message from user 
	printf("%s> ", handleB);							//prompt 
	memset(message, 0, MAX_MESSAGE + 2);
	fflush(stdout);	fflush(stdin);
	fgets(message, MAX_MESSAGE + 2, stdin);			//account for newline in size

	//trim newline 
	message[strcspn(message, "\n")] = 0;
	
	//send message to server
	charsWritten = send(socketfd, message, strlen(message), 0);
	if (charsWritten < 0) { fprintf(stderr,"error: send message\n"); exit(1); };
	
	//check for quit command 
	if (strcmp(quitChat, message) == 0) {
		return 1;
	}
	return 0;
}

/*
**
** Returns 1 to quit chat, 0 to continue 
*/
int receiveMessage(int socketfd, char *reply, char *handleA) {
	int charsRead;
	char *quitChat = "\\quit";
	
	//get reply from server 
	memset(reply, 0, MAX_MESSAGE + 1);
	charsRead =  recv(socketfd, reply, MAX_MESSAGE + 1, 0);
	if (charsRead < 0) { fprintf(stderr,"error: receive message\n"); exit(1); };
	
	//check for quit command 
	if (strcmp(quitChat, reply) == 0) {
		return 1;
	}
	
	//print reply from server 
	printf("%s> %s\n", handleA, reply);
	return 0;

}

//gcc -o chatclient chatclient.c 
//python chatserve.py 
//chatclient flip1.engr.oregonstate.edu

/*
test max size handle: abcdefghij

test max size message: ncSJldGwNHKmfqgxbUxTFTivQSYjrJtABXhTnlnZEBipNxQxRTFAZgrIFaiZlbOnVxHTsRvbCHvBOMTlTWWPqctbPiOQxCXSaJUCpYSBQFyILxxdENazbxXjxBakkEMxuGLiSkTluzVvUGSXdnIDqRPagxFWyiJzffsiBwbHTkmQZgDYZHzbmtpSAmxphUsaSoiEFHqxnRgxAEkEGbLIQtuEvDLiJmgGJNPCzmMdpqNIIuldLgKVwmhwctATMhmHIJZTXgUfJmjtOiqJGuVNJCSomQIUUoCLeOYmFJXRBdxZTLnbmhilVUWXjmyljwzMkYHKBMbhwHiuVjLlgvlyzjQCPJmQhmnTUbISqWAgjgnsxilMmRGswnxAkDThzpuUpPwGthomwtVJGXjipxrUxBucabZJnSQJPBsCDLyWjvrEySQLhRrDgLSitcMHYpZGSzSeXpHigUmMJTolPhfmHfuamfBYUJnyXRYTXCgqSYuRACRCokFb
*/

int main(int argc, char *argv[]) {
	char *hostname, *port;
	char handleA[MAX_HANDLE+1], handleB[MAX_HANDLE+2]; 
	char message[MAX_MESSAGE+2], reply[MAX_MESSAGE+1];

	int status = 0; 

	int socketfd; 

	//check args
	if (argc < 3) { fprintf(stderr,"USAGE: ./chatclient <server-hostname> <port #>\n"); exit(1); } 
	hostname = argv[1];
	port = argv[2];

	/*
	** fill out hints struct, prepare to connect,
	**, make socket, and connect
	*/
	socketfd = initiateContact(hostname, port, status);


	/*
	** exchange handles  
	*/
	exchangeHandles(socketfd, handleA, handleB);
	
	/*
	** run chat until either client or server quits 
	*/
	while(1) { 
		if (sendMessage(socketfd, message, handleB) == 1) {
			printf("quitting\n");
			close(socketfd);
			return 0;
		};
		
		if (receiveMessage(socketfd, reply, handleA) == 1) {
			printf("quitting\n");
			close(socketfd);	
			return 0;
		}
	}

	close(socketfd);

	return 0;
}
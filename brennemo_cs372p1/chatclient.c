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
** Function: 	initiateContact
**
** Description: fills out hints struct, prepares to connect, 
**				makes socket, and connects to server 
** Parameters: 	hostname: server's host name, port: server's port, status: 
** Returns: 	socket file descriptor 
*/
int initiateContact(char *hostname, char *port, int status) {
	struct addrinfo hints, *res;
	char ipstr[INET_ADDRSTRLEN];
	
	int socketfd; 
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET; 	
	hints.ai_socktype = SOCK_STREAM;		//TCP
	hints.ai_flags = AI_PASSIVE; 
	
	//fill addrinfo struct 
	if ((status = getaddrinfo(hostname, port, &hints, &res)) != 0) {
		fprintf(stderr,"error: getaddrinfo: %s\n", gai_strerror(status)); exit(1); 	
	}
	
	//create socket 
	socketfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (socketfd == -1) { fprintf(stderr,"error: socket\n"); exit(1); }
	
	//establish connection 
	status = connect(socketfd, res->ai_addr, res->ai_addrlen);
	if (status == -1) {
		fprintf(stderr,"error: connect\n"); 
		close(socketfd);
		exit(1); 
	}
		
	freeaddrinfo(res);						//free linked list
	
	return socketfd;
	
}

/*
** Function: 	exchangeHandles
** 
** Description: retrieves and stores handles for both client and server users 
** Parameters:  socketfd: , handleA: buffer to hold client user's handle, 
**				handleB: buffer to hold server user's handle 
*/
void exchangeHandles(int socketfd, char *handleA, char *handleB) {
	int charsWritten, charsRead;
	
	//get handle from user 
	printf("Enter handle: ");						//prompt 
	memset(handleB, 0, MAX_HANDLE + 2);				//prepare buffer, stdin, stdout 
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
** Function: 	sendMessage
** 
** Description: displays prompt, gets message from user, sends to server, and 
** 				quits program if user has entered quit command 
** Parameters: socketfd: socket file descriptor, message: buffer to hold user input
				handleB: handle of client/hostB's user 
** Returns:		1 to quit chat, 
				0 to continue 
*/
int sendMessage(int socketfd, char *message, char *handleB) {
	int charsWritten;
	char *quitChat = "\\quit";
	
	//get message from user 
	printf("%s> ", handleB);						//prompt 
	memset(message, 0, MAX_MESSAGE + 2);			//prepare buffer, stdin, stdout 
	fflush(stdout);	fflush(stdin);
	
	fgets(message, MAX_MESSAGE + 2, stdin);			//account for newline in size
	message[strcspn(message, "\n")] = 0;			//trim newline 
	
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
** Function: 	receiveMessage
** 
** Description:	receives message from server, quits program if quit command is received, 
**				or prints message to console in as: "handleA> reply"
** Parameters: 	socketfd: socket file descriptor, reply: buffer to hold server response 
				handleA: handle of server/host A's user 
** Returns:		1 to quit chat, 
				0 to continue 
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


int main(int argc, char *argv[]) {
	char *hostname, *port;
	char handleA[MAX_HANDLE+1], handleB[MAX_HANDLE+2]; 
	char message[MAX_MESSAGE+2], reply[MAX_MESSAGE+1];

	int status = 0; 
	int socketfd; 

	/*
	** check args for correct usage 
	*/
	if (argc < 3) { fprintf(stderr,"USAGE: ./chatclient <server-hostname> <port #>\n"); exit(1); } 
	hostname = argv[1];
	port = argv[2];

	/*
	** establish connection with server 
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
			printf("ending chat and exiting\n");
			close(socketfd);
			return 0;
		};
		
		if (receiveMessage(socketfd, reply, handleA) == 1) {
			printf("ending chat and exiting\n");
			close(socketfd);	
			return 0;
		}
	}

	close(socketfd);

	return 0;
}
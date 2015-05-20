/******************************************************************************
 * Author : Deepak Luitel
 * Echo Client
 * Computer Networks
 * University of Maryland
 * Citations:
 *	1. GNU.org and Man7.org
 *****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>

/*Defining the buffer size and error numbers*/
#define BUFSIZE 1024
#define ERROR -1

/*main starts the program*/
int main(int argc, char *argv[]){
	ssize_t sent = 0; 
	ssize_t received = 0;
	ssize_t sentinbytes = 0; 
	ssize_t receivedbytes = 0;
	char input[BUFSIZE];
	char output[BUFSIZE];
	struct sockaddr_in rem_server;
	struct hostent *hosts;
	int socketfd = socket(AF_INET,SOCK_STREAM,0);

	/*usage error handling*/
	if(argc != 3){
		fprintf(stderr,"USAGE: <echo \"data\"> | %s <host> <port>\n",
		argv[0]);
		exit(1);
	}
	/*socket error handling*/
	if(socketfd == -1){
		perror("ERROR creating socket");
		exit(ERROR);
	}

	/*reset the struct sockaddr_in with NULLS*/
	memset(&rem_server, 0, sizeof(rem_server));
	/*assign the family and port to the struct*/
	rem_server.sin_family = AF_INET;
	rem_server.sin_port = htons(atoi(argv[2]));
	/*get the host by name, google.com -> struct*/
	hosts = (struct hostent *)gethostbyname(argv[1]);
	/*copy the server address*/
	memcpy(&rem_server.sin_addr,hosts->h_addr,sizeof(rem_server.sin_addr));
	
	/*connect to the server*/
	if((connect(socketfd,(struct sockaddr *) &rem_server, 
		sizeof(rem_server))) < 0){
		perror("ERROR connecting to server");
		exit(ERROR);
	}

	/*get and write every 1024 bytes in the socket*/
	while(fgets(input, BUFSIZE, stdin) != NULL){
		if((sentinbytes = write(socketfd, input, strlen(input)))<=0){
			perror("ERROR while writing to socket.");
			exit(ERROR);		
		}
		sent += sentinbytes;
	}

	/* close the writing socket 1 = SHUT_WR*/
	shutdown(socketfd,1);
	
	/*receive the 1024 bytes of chunk and write to the stdout*/ 
	while(received < sent){
		memset(&output[0], 0, sizeof(output));
		if((receivedbytes = read(socketfd, output, BUFSIZE))<=0){
			perror("ERROR reading from socket");
			exit(ERROR);
		}
		received += receivedbytes;
		printf("%s", output);
	}
	
	/*fully close the socket for read (and write) end*/
	close(socketfd);

/*exit the program*/
exit(0);
}


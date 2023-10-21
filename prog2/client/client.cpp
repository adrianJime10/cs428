//UDP Pinger

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <chrono>
#include <sys/time.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>

#define PORT	 11000
#define STRING_LITERALA "X: Ann "
#define STRING_LITERALB "Y: Ben "

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t done = PTHREAD_COND_INITIALIZER;

char Amessage[sizeof(STRING_LITERALA) - 1];
char Bmessage[sizeof(STRING_LITERALB) - 1];

//One of the clients that will send and receive a message from the server
void *clientA(void *data){

	char buffer[1024];
	char message[2048];
	struct sockaddr_in servaddr;
	int sockfd, n;
	socklen_t len;

	//Socket creation
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	
	memset(&servaddr, 0, sizeof(servaddr));
	memcpy(message, STRING_LITERALA, sizeof(message));

	//Fills in required information so that the client may connect to the proper server
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = INADDR_ANY;

	len = sizeof(servaddr);

	//Attempts to connect to the server
	connect(sockfd, (struct sockaddr *) &servaddr, len);

	//Sends the message
	send(sockfd, message, strlen(message),0);
	printf("\nX:Ann Message sent! ------ (X:Ann)\n");

	//Waits until the acknowledgement is received
	n = recv(sockfd, buffer,sizeof(buffer),0);

	printf("\nX:Ann: Server Response --- > %s\n", buffer);


	pthread_exit(NULL);
	//pthread_cond_signal(&done);

	return NULL;
}

//One of the clients that will send and receive a message from the server
void *clientB(void *data){

	char buffer[1024];
	char message[2048];
	struct sockaddr_in servaddr;
	int sockfd, n;
	socklen_t len;

	//Socket creation
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	
	memset(&servaddr, 0, sizeof(servaddr));
	memcpy(message, STRING_LITERALB, sizeof(message));

	//Fills in required information so that the client may connect to the proper server
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = INADDR_ANY;

	len = sizeof(servaddr);

	//Attempts to connect to the server
	connect(sockfd, (struct sockaddr *) &servaddr, len);

	//Sends the message
	send(sockfd, message, strlen(message),0);
	printf("\nY:Ben Message sent! ------ (Y:Ben)\n");

	//Waits until the acknowledgement is received
	n = recv(sockfd, buffer,sizeof(buffer),0);

	printf("\nY:Ben: Server Response --- > %s\n", buffer);


	pthread_exit(NULL);
	//pthread_cond_signal(&done);

	return NULL;
}

int main() {
	
	//Array of pthread ids
	pthread_t tid[2];

	std::cout << "here";

	//Creates the two clients in two separate threads
	pthread_create(&tid[0],NULL,clientA,NULL);
	pthread_create(&tid[1],NULL,clientB,NULL);

	for(int i=0;i<2;i++){
		//Waits for both threads to finish
		pthread_join(tid[i],NULL);
	}	
	return 0;
}

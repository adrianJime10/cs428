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

#define PORT	 12203
#define STRING_LITERAL "Hello There!"

pthread_mutex_t calculating = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t done = PTHREAD_COND_INITIALIZER;

int sockfd, n;
socklen_t len;
char message[sizeof(STRING_LITERAL) - 1];
char buffer[1024];
struct sockaddr_in servaddr;

//Sends a message to designated server address
void *sendMessage(void *data){
	
	int oldtype;

	//Allows this thread to be canceled at any time
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldtype);

	//Sends the message and starts the timer for RTT
	auto start = std::chrono::high_resolution_clock::now();
	sendto(sockfd, (const char *) message, strlen(message), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
	//printf("Message sent! ------ ");

	//Waits until the acknowledgement is received (or until timeout) and ends the timer for RTT
	n = recvfrom(sockfd, (char *)buffer,sizeof(buffer), MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
	auto stop = std::chrono::high_resolution_clock::now(); 
	printf("\nServer: %s\n", buffer);

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);

	std::cout << "RTT: " << duration.count() << " microseconds\n\n";

	//Signals the end of this thread
	pthread_cond_signal(&done);

	return NULL;
}

int main() {
	
	//Pthread and max wait time declarations
	pthread_t tid;
	int err;
	struct timespec max_wait;

	//Socket creation
	sockfd = socket(AF_INET,SOCK_DGRAM,0);

	memset(&max_wait,0,sizeof(max_wait));
	memset(&servaddr, 0, sizeof(servaddr));
	memcpy(message, STRING_LITERAL, sizeof(message));

	//Assigns the maximum wait time to one second
	max_wait.tv_sec=1;

	//Fills in required information so that the client may connect to the proper server
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = INADDR_ANY;

	for(int i=0;i<10;i++){
		//Sets the maximum wait time to max_wait.tv_sec (which was assigned to 1 second)
		//and starts the timer
		struct timespec abs_time;
		clock_gettime(CLOCK_REALTIME, &abs_time);
		abs_time.tv_sec += max_wait.tv_sec;
		abs_time.tv_nsec += max_wait.tv_nsec;

		//Creates a new thread
		pthread_create(&tid,NULL,sendMessage,NULL);

		//Checks to see if the thread timed out, kills it if so
		err = pthread_cond_timedwait(&done, &calculating, &abs_time);
		if(err == ETIMEDOUT){
			fprintf(stderr, "-----Timed out-----\n");
		}

		//Terminates the thread
		pthread_cancel(tid);
		pthread_join(tid,NULL);
		tid++;
	}	
	return 0;
}

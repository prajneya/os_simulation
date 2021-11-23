#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include<pthread.h>
#include <errno.h>

typedef struct Command{
    int uid;

    int time;
    char name[50];
    int key1;
    int key2;
    char val[100];
} Command;

Command ** commands;

pthread_t * command_t;

void * commandRunner(void *a){

	Command * c = (Command *)a;

	printf("In thread\n");

	char buffer[1024];

	int clientSocket;
	struct sockaddr_in serverAddr;
	socklen_t addr_size;

	// Create the socket. 
	clientSocket = socket(AF_UNIX, SOCK_STREAM, 0);
	printf("SOCKET: %d\n", clientSocket);

	//Configure settings of the server address
	// Address family is Internet 
	serverAddr.sin_family = AF_UNIX;

	//Set port number, using htons function 
	serverAddr.sin_port = htons(2325);

	//Set IP address to localhost
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

	//Connect the socket to the server using the address
	addr_size = sizeof serverAddr;
	int conn = connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
	printf("CONNECTION: %d\n", conn);


	int sendVal = send(clientSocket , c->name , strlen(c->name) , 0); 
	printf("Send %d\n", sendVal);
	
	//Read the message from the server into the buffer
	int recVal = recv(clientSocket, buffer, 1024, 0);
	printf("Receive %d\n", recVal);


	// printf("Data received: %s\n",buffer);
	int closeVal = close(clientSocket);
	printf("Close %d\n", closeVal);

	printf("CLOSING THREAD\n");
	pthread_exit(NULL);
}

int main(){
    int m;
    scanf("%d", &m);

    commands = (Command **) malloc(sizeof(Command *) * m);

    for(int i = 0; i < m; i++){
    	Command * c = (Command *) malloc(sizeof(Command));
    	scanf(" %d", &c->time);

    	char command[50];
    	scanf("%s", command);

    	if(!strcmp(command, "insert")){
    		scanf("%d %s", &c->key1, c->val);
    	}
    	else if(!strcmp(command, "delete")){
    		scanf("%d", &c->key1);
    	}
    	else if(!strcmp(command, "update")){
    		scanf("%d %s", &c->key1, c->val);
    	}
    	else if(!strcmp(command, "concat")){
    		scanf("%d %d", &c->key1, &c->key2);
    	}
    	else if(!strcmp(command, "fetch")){
    		scanf("%d", &c->key1);
    	}
    	commands[i] = c;
    }

    command_t = (pthread_t *) malloc(sizeof(pthread_t) * m);
    for(int i = 0; i < m; i++){
    	pthread_create(&command_t[i], 0, commandRunner, commands[i]);
    }

    int i = 0;
	sleep(5);
	while(i<m){
		pthread_join(command_t[i++],NULL);
		printf("%d:\n",i);
	}
}
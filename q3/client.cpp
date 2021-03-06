#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
 
// inet_addr
#include <arpa/inet.h>
#include <unistd.h>
 
// For threading, link with lpthread
#include <pthread.h>
#include <semaphore.h>

#include <iostream>
using namespace std;

typedef struct Command{
	int uid;
    int time;
    int command_type;
    // 0 insert
    // 1 delete
    // 2 update
    // 3 contact
    // 4 fetch
    int key1;
    int key2;
    char val[100];
} Command;

Command ** commands;

pthread_t * command_t;

unsigned char * serialize_int(unsigned char *buffer, int value)
{
  /* Write big-endian int value into buffer; assumes 32-bit int and 8-bit char. */
  buffer[0] = value >> 24;
  buffer[1] = value >> 16;
  buffer[2] = value >> 8;
  buffer[3] = value;
  return buffer + 4;
}

unsigned char * serialize_string(unsigned char *buffer, int size, char *value)
{
  int i = 0;
  for(i = 0; i < size; i++){
  	buffer[i] = value[i];
  }
  return buffer + i;
}

unsigned char * serialize_command(unsigned char *buffer, struct Command *value)
{
  buffer = serialize_int(buffer, value->time);
  buffer = serialize_int(buffer, value->command_type);
  buffer = serialize_int(buffer, value->key1);
  buffer = serialize_int(buffer, value->key2);
  buffer = serialize_int(buffer, strlen(value->val));
  buffer = serialize_string(buffer, strlen(value->val), value->val);
  return buffer;
}

int send_Command(int socket, struct Command *command)
{
  unsigned char buffer[512], *ptr;

  ptr = serialize_command(buffer, command);

  return send(socket, buffer, ptr - buffer, 0);
}

 
// Function to send data to
// server socket.
void* commandRunner(void* a)
{
 
    Command * c = (Command *)a;

    int network_socket;
 
    // Create a stream socket
    network_socket = socket(AF_INET,
                            SOCK_STREAM, 0);
 
    // Initialise port number and address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8989);
 	
 	// sleep for time allocated
 	sleep(c->time);
    // Initiate a socket connection
    int connection_status = connect(network_socket,
                                    (struct sockaddr*)&server_address,
                                    sizeof(server_address));
 
    // Check for connection error
    if (connection_status < 0) {
        puts("Error\n");
        return 0;
    }
 
    // printf("%d Connection established\n", c->uid);
    // Send data to the socket
    int sendVal = send_Command(network_socket, c);

    // cout << "SENT " << sendVal << " BYTES" << endl;

    unsigned char buffer[512];
    int recVal = recv(network_socket, buffer, sizeof(buffer), 0);

    int errcode = (buffer[3]<<0) | (buffer[2]<<8) | (buffer[1]<<16) | (buffer[0]<<24);
    int thread_id = (buffer[7]<<0) | (buffer[6]<<8) | (buffer[5]<<16) | (buffer[4]<<24);
    int len_val = (buffer[11]<<0) | (buffer[10]<<8) | (buffer[9]<<16) | (buffer[8]<<24);

    int z = 0;
    char ret_value[200];

    for(z = 12; z<12+len_val; z++){
    	ret_value[z-12] = buffer[z];
    }

    // cout << "RECEIVED " << recVal << " BYTES" << endl;

    if(errcode==0){
    	cout << c->uid << ":" << thread_id << ":Insertion Successful" << endl;
    }
    else if(errcode==1){
    	cout << c->uid << ":" << thread_id << ":Key already exists" << endl;
    }
    else if(errcode==2){
    	cout << c->uid << ":" << thread_id << ":No such key exists" << endl;
    }
    else if(errcode==3){
    	cout << c->uid << ":" << thread_id << ":Deletion Successful" << endl;
    }
    else if(errcode==4){
    	cout << c->uid << ":" << thread_id << ":Key does not exist" << endl;
    }
    else if(errcode==5){
    	printf("%d:%d:%s\n", c->uid, thread_id, ret_value);
    }
    else if(errcode==6){
    	cout << c->uid << ":" << thread_id << ":Concat failed as at least one of the keys does not exist" << endl;
    }
    else if(errcode==7){
    	printf("%d:%d:%s\n", c->uid, thread_id, ret_value);
    }
    else if(errcode==8){
    	cout << c->uid << ":" << thread_id << ":Key does not exist" << endl;
    }
    else if(errcode==9){
    	printf("%d:%d:%s\n", c->uid, thread_id, ret_value);
    }
 
    // Close the connection
    close(network_socket);

    pthread_exit(NULL);
 
    return 0;
}
 
// Driver Code
int main()
{

	int m;
    scanf("%d", &m);

    commands = (Command **) malloc(sizeof(Command *) * m);

    for(int i = 0; i < m; i++){
    	Command * c = (Command *) malloc(sizeof(Command));
    	scanf(" %d", &c->time);

    	c->uid = i;

    	char command[50];
    	scanf("%s", command);

    	if(!strcmp(command, "insert")){
    		c->command_type = 0;
    		scanf("%d %s", &c->key1, c->val);

    		c->key2 = 0;
    	}
    	else if(!strcmp(command, "delete")){
    		c->command_type = 1;
    		scanf("%d", &c->key1);

    		c->key2 = 0;
    		strcpy(c->val, "NULL");
    	}
    	else if(!strcmp(command, "update")){
    		c->command_type = 2;
    		scanf("%d %s", &c->key1, c->val);

    		c->key2 = 0;
    	}
    	else if(!strcmp(command, "concat")){
    		c->command_type = 3;
    		scanf("%d %d", &c->key1, &c->key2);

    		strcpy(c->val, "NULL");
    	}
    	else if(!strcmp(command, "fetch")){
    		c->command_type = 4;
    		scanf("%d", &c->key1);

    		c->key2 = 0;
    		strcpy(c->val, "NULL");
    	}
    	commands[i] = c;
    }

    command_t = (pthread_t *) malloc(sizeof(pthread_t) * m);
    for(int i = 0; i < m; i++){
    	pthread_create(&command_t[i], 0, commandRunner, commands[i]);
    }

    int i = 0;
	// sleep(5);
	while(i<m){
		pthread_join(command_t[i++],NULL);
		// printf("%d:\n",i);
	}
}
// C program for the Server Side
 
// inet_addr
#include <arpa/inet.h>
 
// For threading, link with lpthread
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>

#include <bits/stdc++.h>
#include <map>
using namespace std;
 
// Semaphore variables
sem_t x[128];
pthread_t tid;
pthread_t insertthreads[100];
pthread_t deletethreads[100];
pthread_t updatethreads[100];
pthread_t concatthreads[100];
pthread_t fetchthreads[100];

//dictionary
map<int, string> m;

typedef struct Command{
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

typedef struct threadArg{
	Command * command;
	int socket;
} threadArg;

typedef struct ErrorCode{
	int error;
	char value[200];
	pid_t thread_id;
} ErrorCode;

unsigned char * serialize_int(unsigned char *buffer, int value)
{
  /* Write big-endian int value into buffer; assumes 32-bit int and 8-bit char. */
  buffer[0] = value >> 24;
  buffer[1] = value >> 16;
  buffer[2] = value >> 8;
  buffer[3] = value;
  return buffer + 4;
}

unsigned char * serialize_string(unsigned char *buffer, int size, char * value)
{
  int i = 0;
  for(i = 0; i < size; i++){
  	buffer[i] = value[i];
  }
  return buffer + i;
}

unsigned char * serialize_command(unsigned char *buffer, struct ErrorCode *value)
{
  buffer = serialize_int(buffer, value->error);
  buffer = serialize_int(buffer, value->thread_id);
  buffer = serialize_int(buffer, strlen(value->value));
  buffer = serialize_string(buffer, strlen(value->value), value->value);
  return buffer;
}

int sendErrorCommand(int socket, struct ErrorCode *error)
{
  unsigned char buffer[512], *ptr;

  ptr = serialize_command(buffer, error);

  return send(socket, buffer, ptr - buffer, 0);
}


// Insert Function
void* insertRunner(void* a)
{

	threadArg * allargs = (threadArg *) a;
	Command * c = (Command *) allargs->command;
	int network_socket = allargs->socket;

    // Lock the semaphore
    sem_wait(&x[c->key1]);

    ErrorCode * e = (ErrorCode *) malloc(sizeof(ErrorCode));
    strcpy(e->value, "");
    e->thread_id = gettid();

    // cout << "ATTEMPT " << c->command_type << " " << c->key1 << " " << c->val << endl;

    if (m.find(c->key1) == m.end()){
    	e->error = 0;
    	m[c->key1] = c->val;
    	sleep(2);
    	sendErrorCommand(network_socket, e);
    }
    else{
    	e->error = 1;
    	sleep(2);
    	sendErrorCommand(network_socket, e);
    }
 
    // Unlock the semaphore
    sem_post(&x[c->key1]);
    pthread_exit(NULL);
}
 
// Delete Function
void* deleteRunner(void* a)
{
    threadArg * allargs = (threadArg *) a;
	Command * c = (Command *) allargs->command;
	int network_socket = allargs->socket;
    // Lock the semaphore
    sem_wait(&x[c->key1]);

    ErrorCode * e = (ErrorCode *) malloc(sizeof(ErrorCode));
    strcpy(e->value, "");
    e->thread_id = gettid();

    if (m.find(c->key1) == m.end()){
    	e->error = 2;
    	sleep(2);
    	sendErrorCommand(network_socket, e);
    }
    else{
    	e->error = 3;
    	m.erase(c->key1);
    	sleep(2);
    	sendErrorCommand(network_socket, e);
    }
 
    // Unlock the semaphore
    sem_post(&x[c->key1]);
    pthread_exit(NULL);
}

// Update Function
void* updateRunner(void* a)
{
	threadArg * allargs = (threadArg *) a;
	Command * c = (Command *) allargs->command;
	int network_socket = allargs->socket;
    // Lock the semaphore
    sem_wait(&x[c->key1]);

    ErrorCode * e = (ErrorCode *) malloc(sizeof(ErrorCode));
    strcpy(e->value, "");
    e->thread_id = gettid();

    if (m.find(c->key1) == m.end()){
    	e->error = 4;
    	sleep(2);
    	sendErrorCommand(network_socket, e);
    }
    else{
    	e->error = 5;
    	m[c->key1] = c->val;
    	strcpy(e->value, c->val);
    	sleep(2);
    	sendErrorCommand(network_socket, e);
    }
 
    // Unlock the semaphore
    sem_post(&x[c->key1]);
    pthread_exit(NULL);
}

// Concat Function
void* concatRunner(void* a)
{
	threadArg * allargs = (threadArg *) a;
	Command * c = (Command *) allargs->command;
	int network_socket = allargs->socket;

    // Lock the semaphore
    sem_wait(&x[c->key1]);
    sem_wait(&x[c->key2]);

    ErrorCode * e = (ErrorCode *) malloc(sizeof(ErrorCode));
    strcpy(e->value, "");
    e->thread_id = gettid();

    if (m.find(c->key1) == m.end() || m.find(c->key2) == m.end()){
    	e->error = 6;
    	sleep(2);
    	sendErrorCommand(network_socket, e);
    }
    else{
    	e->error = 7;

    	string fstring = "";

    	for(int i = 0; i < m[c->key1].length(); i++){
    		fstring += m[c->key1][i];
    	}

    	string lstring = "";

    	for(int i = 0; i < m[c->key2].length(); i++){
    		lstring += m[c->key2][i];
    	}

    	m[c->key1] = fstring+lstring;
    	m[c->key2] = lstring+fstring;

    	strcpy(e->value, m[c->key2].c_str());
    	sleep(2);

    	sendErrorCommand(network_socket, e);
    }
 
    // Unlock the semaphore
    sem_post(&x[c->key1]);
    sem_post(&x[c->key2]);
    pthread_exit(NULL);
}

// Fetch Function
void* fetchRunner(void* a)
{
	threadArg * allargs = (threadArg *) a;
	Command * c = (Command *) allargs->command;
	int network_socket = allargs->socket;
    // Lock the semaphore
    sem_wait(&x[c->key1]);

    ErrorCode * e = (ErrorCode *) malloc(sizeof(ErrorCode));
    strcpy(e->value, "");
    e->thread_id = gettid();

    if (m.find(c->key1) == m.end()){
    	e->error = 8;
    	sleep(2);
    	sendErrorCommand(network_socket, e);
    }
    else{
    	e->error = 9;
    	strcpy(e->value, m[c->key1].c_str());
    	sleep(2);
    	sendErrorCommand(network_socket, e);
    }
 
    // Unlock the semaphore
    sem_post(&x[c->key1]);
    pthread_exit(NULL);
}
 
// Driver Code
int main()
{
    // Initialize variables
    int serverSocket, newSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
 
    socklen_t addr_size;

    for(int z = 0; z<128; z++)
    	sem_init(&x[z], 0, 1);
 
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8989);
 
    // Bind the socket to the
    // address and port number.
    bind(serverSocket,
         (struct sockaddr*)&serverAddr,
         sizeof(serverAddr));
 
    // Listen on the socket,
    // with 40 max connection
    // requests queued
    if (listen(serverSocket, 50) == 0)
        printf("Listening\n");
    else
        printf("Error\n");

    // Array for thread
    pthread_t tid[60];
 
    int i = 0;
 
    while (1) {
        addr_size = sizeof(serverStorage);
 
        // Extract the first
        // connection in the queue
        newSocket = accept(serverSocket,
                           (struct sockaddr*)&serverStorage,
                           &addr_size);

        unsigned char buffer[512];

        int recVal = recv(newSocket, buffer, sizeof(buffer), 0);

        // cout << "RECEIVED " << recVal << " BYTES" << endl;

        Command * c = (Command *) malloc(sizeof(Command));

        c->time = (buffer[3]<<0) | (buffer[2]<<8) | (buffer[1]<<16) | (buffer[0]<<24);
        c->command_type = (buffer[7]<<0) | (buffer[6]<<8) | (buffer[5]<<16) | (buffer[4]<<24);

        int len_key1, len_key2, len_val;
        
        c->key1 = (buffer[11]<<0) | (buffer[10]<<8) | (buffer[9]<<16) | (buffer[8]<<24);
        c->key2 = (buffer[15]<<0) | (buffer[14]<<8) | (buffer[13]<<16) | (buffer[12]<<24);

        len_val = (buffer[19]<<0) | (buffer[18]<<8) | (buffer[17]<<16) | (buffer[16]<<24);

        for(int z=20; z<20+len_val; z++){
        	c->val[z-20] = buffer[z];
        }

        // cout << c->time << " - " << c->command_type << " - "  << c->key1 << " - "  << c->key2 << " - "  << c->val << endl;

        threadArg * ta = (threadArg *) malloc(sizeof(threadArg));
        ta->command = c;
        ta->socket = newSocket;

        // cout << "THREAD" << ta->command->time << " " << ta->command->command_type << " "  << ta->command->key1 << " "  << ta->command->key2 << " "  << ta->command->val << endl;

        if (c->command_type == 0) {
            // Creater insert thread
            if (pthread_create(&insertthreads[i++], NULL,
                               insertRunner, ta)
                != 0)
 
                // Error in creating thread
                printf("Failed to create thread\n");
        }
        else if (c->command_type == 1) {
            // Create delete thread
            if (pthread_create(&deletethreads[i++], NULL,
                               deleteRunner, ta)
                != 0)
 
                // Error in creating thread
                printf("Failed to create thread\n");
        }
         else if (c->command_type == 2) {
            // Create update thread
            if (pthread_create(&updatethreads[i++], NULL,
                               updateRunner, ta)
                != 0)
 
                // Error in creating thread
                printf("Failed to create thread\n");
        }
         else if (c->command_type == 3) {
            // Create concat thread
            if (pthread_create(&concatthreads[i++], NULL,
                               concatRunner, ta)
                != 0)
 
                // Error in creating thread
                printf("Failed to create thread\n");
        }
         else if (c->command_type == 4) {
            // Create fetch thread
            if (pthread_create(&fetchthreads[i++], NULL,
                               fetchRunner, ta)
                != 0)
 
                // Error in creating thread
                printf("Failed to create thread\n");
        }
        else{
        	cout << "INVALID COMMAND" << endl;
        }

        sleep(1);
 
        if (i >= 50) {
            // Update i
            i = 0;
 
            while (i < 50) {
                // Suspend execution of
                // the calling thread
                // until the target
                // thread terminates
                pthread_join(insertthreads[i++],
                             NULL);
                pthread_join(deletethreads[i++],
                             NULL);
                pthread_join(updatethreads[i++],
                             NULL);
                pthread_join(concatthreads[i++],
                             NULL);
                pthread_join(fetchthreads[i++],
                             NULL);
            }
 
            // Update i
            i = 0;
        }
    }
 
    return 0;
}
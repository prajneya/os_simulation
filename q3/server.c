#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <arpa/inet.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include<pthread.h>
#include <errno.h>

#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)

char client_message[2000];
char buffer[1024];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void * socketThread(void *arg)
{
  int newSocket = *((int *)arg);
  printf("SOCKET: %d\n", newSocket);
  int recvValue = recv(newSocket , client_message , 2000 , 0);
  printf("Receive %d\n", recvValue);
  if(recvValue == -1){
    handle_error("recv");
    printf("Error receiving message from client :(\n");
  }
  else{
    printf("%s\n", client_message);
  }

  // Send message to the client socket 
  pthread_mutex_lock(&lock);
  char *message = malloc(sizeof(client_message)+20);
  strcpy(message,"Hello Client : ");
  strcat(message,client_message);
  strcat(message,"\n");
  strcpy(buffer,message);
  free(message);
  pthread_mutex_unlock(&lock);
  
  sleep(1);
  int sendValue = send(newSocket,buffer,13,0);
  if(sendValue == -1){
    handle_error("send");
    printf("Error sending message to client :(\n");
  }
  printf("Exit socketThread \n");
  close(newSocket);
  pthread_exit(NULL);
}

int main(){
  int serverSocket;
  struct sockaddr_in serverAddr;
  socklen_t addr_size;

  //Create the socket. 
  serverSocket = socket(AF_UNIX, SOCK_STREAM, 0);

  // Configure settings of the server address struct
  // Address family = Internet 
  serverAddr.sin_family = AF_UNIX;

  //Set port number, using htons function to use proper byte order 
  serverAddr.sin_port = htons(2325);

  //Set IP address to localhost 
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");


  //Set all bits of the padding field to 0 
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

  //Bind the address struct to the socket 
  int bindValue = bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
  if (bindValue == -1)
    handle_error("bind");

  //Listen on the socket, with 50 max connection requests queued 
  if(listen(serverSocket,50)==0)
    printf("Listening\n");
  else
    printf("Error\n");

  pthread_t tid[60];
  int i = 0;

  int newSockets[60];

  while(1){
      struct sockaddr_in newServerAddr;
      //Accept call creates a new socket for the incoming connection
      addr_size = sizeof newServerAddr;

      int newSocket;

      newSockets[i] = accept(serverSocket, (struct sockaddr *) &newServerAddr, &addr_size);

      //for each client request creates a thread and assign the client request to it to process
     //so the main thread can entertain next request
      if( pthread_create(&tid[i], NULL, socketThread, &newSockets[i]) != 0 )
         printf("Failed to create thread\n");

      i++;
      
      if( i >= 50){
        i = 0;
        while(i < 50){
          pthread_join(tid[i++],NULL);
        }
        i = 0;
      }
  }
  return 0;
}
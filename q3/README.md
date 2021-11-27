# Question 3: MultiThreaded Client and Server

Implementation of a multithreaded client and server program where one server receives requests from multiple clients simultaneously, coded in C++.

## Implementation

### Client

I maintain an array of commands, where each command is declared in a struct. Each command is allotted a UID for request index and according to the command being given in the input, key(s) and value is taken input. For each command, a thread is created, and each thread sleeps for the time after which the request has to be established. After sleeping, the thread makes a connection with the server, and sends a buffer packet, which is basically the command struct serialized into an unsigned char array.

Each command is sent as a command code, which correspond to the following:

* 0: INSERT
* 1: DELETE
* 2: UPDATE
* 3: CONCAT
* 4: FETCH

---

* The (0-3) bytes of the sent buffer packet consist of the big endian value of the command time.
* The (4-7) bytes of the sent buffer packet consist of the big endian value of the command type based on the above codes.
* The (8-11) bytes of the sent buffer packet consist of the big endian value of ```key_1```.
* The (12-15) bytes of the sent buffer packet consist of the big endian value of ```key_2```.
* The (16-19) bytes of the sent buffer packet consist of the big endian value of length of ```value```.
* The next ```strlen(value)``` bytes consist of the actual ```value```.

After sending the buffer packet, the client receives another buffer packet from the server which contains information about the request received. It contains an error code each of which correspond to the following:

* 0: INSERTION: Successful
* 1: INSERTION: Failed, key already exists
* 2: DELETION: Failed, no such key exists
* 3: DELETION: Successful
* 4: UPDATE: Failed, key does not exist
* 5: UPDATE: Successful
* 6: CONCAT: Failed, one of the keys do not exist
* 7: CONCAT: Successful
* 8: FETCH: Failed, Key does not exist
* 9: FETCH: Successful

The received buffer packet also contains the thread_id which was allotted to the request and the verdict in case the request is successful and requires a verdict.

### Server

The server starts listening to the specified amount of ```max_connections``` from the argument line. Upon an incoming connection, it deserializes the received buffer packet and creates a thread according to the command received. 5 different thread functions are defined for each of the 5 commands.

A semaphore array of 128 items is created to keep a check of the keys being utilised. Each thread function locks the semaphore corresponding to the key index it utilises, hence threads working on different keys can function parallely.

A priority queue is maintained to keep a check of threads executing according to time,that is, threads which are scheduled for t_1 > t_2, should only be executed after thread at t_2 has completed execution.

Each thread upon completing its function sleeps for 2 seconds before sending a response to the client.

## Assumptions

* Large input is to be avoided, since a large number of threads cause a problem.
* All ints are assumed to be 32-bit int and all chars are assumed to be 8-bit char.
* Each command should be less than 512 bytes
* 8989 port is utilised to connect

## Usage

```bash
make clean
```

To remove all unnecessary output files

```bash
g++ server.cpp -o server -lpthread
```

To compile server files and

```bash
g++ client.cpp -o client -lpthread
```

To compile client files.

```bash
./server n
```

To run the server, where n is the maximum number of worker threads to allot

```bash
./client
```

To run the client and provide appropriate input

Preferably run the server and client on different terminals.
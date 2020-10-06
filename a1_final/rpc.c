/*

COMP 310 / ECSE 427 - Operating Systems
Assignment 1
Felix Simard (260865674)

*/

#include "a1_lib.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <unistd.h>
#include <sys/wait.h>

#include "helper.h"

#define BUFSIZE 1024
#define NUM_CONNECTIONS_ALLOWED 5
#define SHARED_FILE "pids.txt"

/*--- CREATE SERVER ---*/
int RPC_Init(const char *host, uint16_t port, int *sockfd) {
  return create_server(host, port, sockfd);
}

/*--- CLOSE SERVER ---*/
int RPC_Close(int sockfd) {
  return close(sockfd);
}

/*--- CONNECT TO SERVER ---*/
int RPC_Connect(const char *host, uint16_t port, int *sockfd) {
  return connect_to_server(host, port, sockfd);
}

/*--- ACCEPT CONNECTIONS ---*/
int RPC_Accept(int sockfd, int *clientfd) {
  return accept_connection(sockfd, clientfd);
}

/*--- SEND MESSAGE ---*/
ssize_t RPC_Call(int sockfd, const char *buf, size_t len) {
  return send_message(sockfd, buf, len);
}

/*--- RECEIVE MESSAGE ---*/
ssize_t RPC_Receive(int sockfd, char *buf, size_t len) {
  return recv_message(sockfd, buf, len);
}


/*--------- CALCULATOR METHODS ----------*/

/*--- addInts a b ---*/
int add(int a, int b) {
  return a + b;
}

/*--- multiplyInts a b ---*/
int multiply(int a, int b) {
  return a * b;
}

/*--- divideFloats a b ---*/
float divide(float a, float b) {
  return a / b;
}

/*--- sleep x ---*/
int mySleep(int x) {
  sleep(x);
  return 0;
}

/*--- factorial x ---*/
uint64_t factorial(int x) {
  if(x >= 1) {
    return x*factorial(x-1);
  } else {
    return 1;
  }
}

/*----------------------------------------*/

/*--- SERVE CLIENT ---*/
int RPCServ(int clientfd) {

  // Init variables
  char msg[BUFSIZE];
  char response[BUFSIZE];

  bool quitRequested = false;

  char line[BUFSIZE];

  // Shared file pointer
  FILE *shared_file;

  while (strcmp(msg, "exit\n")) {

    // Result variable
    char result[BUFSIZE];

    // Allocate memory
    memset(msg, 0, sizeof(msg));
    memset(msg, 0, sizeof(msg));

    // Receive message from client
    ssize_t byte_count = RPC_Receive(clientfd, msg, BUFSIZE);
    if (byte_count <= 0) {
      break;
    }

    // Clean user input and parse
    char* msg_clean;
    msg_clean = cleanInput(msg);
    char** inputs;
    inputs = parseMessage(msg_clean); // returns an indexed array of the inputted arguments

    // Store variables
    char* action = inputs[0];
    char* param1 = inputs[1];
    char* param2 = inputs[2];

    // Determine which action to take based off command inputed by user
    if(strcmp(action, "add") == 0) { // ADD

      int x = atoi(param1);
      int y = atoi(param2);
      int r = add(x, y);

      sprintf(result, "%d", r);

    } else if(strcmp(action, "multiply") == 0) { // MULTIPLY

      int x = atoi(param1);
      int y = atoi(param2);
      int r = multiply(x, y);

      sprintf(result, "%d", r);

    } else if(strcmp(action, "divide") == 0) { // DIVIDE

      float x = atof(param1);
      float y = atof(param2);

      if(y == 0) { // check for division by 0
        sprintf(result, "%s", "Error: Division by zero");
      } else {
        float r = divide(x, y);
        sprintf(result, "%f", r);
      }

    } else if(strcmp(action, "factorial") == 0) { // FACTORIAL

      int x = atoi(param1);
      uint64_t r = factorial(x);
      sprintf(result, "%ld", r);

    } else if(strcmp(action, "sleep") == 0) { // SLEEP

      int x = atoi(param1);
      mySleep(x);

    } else if(strcmp(action, "exit") == 0) { // EXIT

      sprintf(result, "Bye!");
      // we close the the clientfd after sending message back to frontend

    } else if(strcmp(action, "quit") == 0) { // QUIT / SHUTDOWN

      sprintf(result, "Bye!");
      quitRequested = true;
      break;

    } else { // unsupported command

      sprintf(result, "Error: Command '%s' not found", action);

    }

    // Send response to frontend
    RPC_Call(clientfd, result, BUFSIZE);

    // Flush the stdout
    *result = '\0';
    fflush(stdout);

    if(strcmp(action, "exit") == 0) {
      RPC_Close(clientfd); // decreases connections by 1
    }


  }

  if(quitRequested) {

    printf("\nQuit Requested.\n");

    // Reopen shared file
    shared_file = fopen(SHARED_FILE, "r");
    fgets(line, sizeof(line), shared_file);
    fclose(shared_file);
    line[0] = '0'; // change flag to 0 = stop

    // Now, write back ton shared file
    shared_file = fopen(SHARED_FILE, "w");
    fprintf(shared_file, "%s", line);
    fclose(shared_file);

    // Close this client file descriptor
    RPC_Close(clientfd);

  }

  return 0;

}

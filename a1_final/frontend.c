/*

COMP 310 / ECSE 427 - Operating Systems
Assignment 1
Felix Simard (260865674)

*/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "helper.h" // our custom helper functions for parsing inputs and other

#include "rpc.h" // our RPC methods

#define BUFSIZE 1024

int main(int argc, char *argv[]) {
  int sockfd;
  char user_input[BUFSIZE] = { 0 };
  char server_msg[BUFSIZE] = { 0 };

  // Command line arguments
  char host[BUFSIZE];
  int port;

  // Flag to see if the loop should keep running
  bool keepRunning = true;

  if(argc < 3) { // if host, port not provided, use default in helper.h
    strcpy(host, HOST);
    port = PORT;
  } else {
    sscanf(argv[1], "%s", host);
    sscanf(argv[2], "%d", &port);
  }
  //


  // Connect to server
  if (RPC_Connect(host, port, &sockfd) < 0) {
    fprintf(stderr, "\n");
    return -1;
  }

  ssize_t byte_count = RPC_Receive(sockfd, server_msg, sizeof(server_msg));
  if(strcmp(server_msg, "n") == 0) { // refused connection
    printf("The server has been shutdown.\n");
    return 0;
  }

  while (1) {
    memset(user_input, 0, sizeof(user_input));
    memset(server_msg, 0, sizeof(server_msg));


    // Prompt user for input
    printf(">> ");

    // Fetch the input from the user (from stdin)
    fgets(user_input, BUFSIZE, stdin);

    // Send the input to server
    RPC_Call(sockfd, user_input, strlen(user_input));

    // Receive a message from the server
    byte_count = RPC_Receive(sockfd, server_msg, sizeof(server_msg));
    if (byte_count <= 0) {
      break;
    }

    // Now, print out the response from the server
    printf("%s\n", server_msg);

    // Now check if the message initiated an exit or quit command
    if(strcmp(user_input, "quit\n") == 0 || strcmp(user_input, "exit\n") == 0) {
      break;
    }

  }

  return 0;
}

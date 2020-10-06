/*

COMP 310 / ECSE 427 - Operating Systems
Assignment 1
Felix Simard (260865674)

*/
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include  <sys/types.h>
#include<sys/wait.h>

#include "a1_lib.h"

#include "helper.h" // our custom helper functions for parsing inputs and other

#define BUFSIZE 50
#define NUM_CONNECTIONS_ALLOWED 5

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

/* factorial x */
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

  while (strcmp(msg, "exit\n")) {

    // Result variable
    char result[BUFSIZE];

    // Allocate memory
    memset(msg, 0, sizeof(msg));
    memset(msg, 0, sizeof(msg));

    // Receive message from client
    ssize_t byte_count = recv_message(clientfd, msg, BUFSIZE);
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

    } else if(strcmp(action, "quit") == 0) { // QUIT

      sprintf(result, "Bye!");

    } else { // unsupported command

      sprintf(result, "Error: Command '%s' not found", action);

    }

    // Send response to frontend
    send_message(clientfd, result, BUFSIZE);

    // Flush the stdout
    *result = '\0';
    fflush(stdout);

    // Now check if the message initiated an exit or quit command
    if(strcmp(action, "quit") == 0) {
      quitRequested = true;
      break;
    }

  }

  if(quitRequested) {
    printf("\nQuit Request\n");
    exit(1); // pass exit value to parent process
  }

  return 0;
}


/*--- MAIN ---*/
int main(int argc, char *argv[]) {

  // Variables initialization
  int sockfd, clientfd;
  int pid, returned_value;
  int num_connections = 0;

  // Lists to hold client file descriptors and children process IDs
  int child_pids[NUM_CONNECTIONS_ALLOWED];
  int frontend_fds[NUM_CONNECTIONS_ALLOWED];

  // Command line arguments
  char host[BUFSIZE];
  int port;

  if(argc < 3) { // if host, port not provided, use default in helper.h
    strcpy(host, HOST);
    port = PORT;
  } else {
    sscanf(argv[1], "%s", host);
    sscanf(argv[2], "%d", &port);
  }
  //


  // Create server with HOST and PORT (see helper.h)
  if (create_server(host, port, &sockfd) < 0) {
    perror("Error creating server\n");
    return -1;
  }
  //

  // Backend Loop
  while(1) {

    printf("\nAvailable for connections...\n");

    // Accept connections from frontend clients
    if (accept_connection(sockfd, &clientfd) < 0) {
      perror("Error accepting connection.\n");
      break;
    }
    printf("\nConnection received. Client FD: %d\n", clientfd);
    //

    // FORKING
    pid = fork();
    if(pid == 0 && num_connections != NUM_CONNECTIONS_ALLOWED) { // child process, serve the client

      // Serve the cient process
      RPCServ(clientfd);

      //num_connections = (num_connections - 1) >= 0 ? (num_connections - 1) : 0; // decrease the number of connections when a child process ends

    } else if(pid > 0) { // parent process

      // Insert child process and client file descriptors in respective lists
      child_pids[num_connections] = pid;
      frontend_fds[num_connections] = clientfd;

      for(int i=0; i <= num_connections; i++) {
        waitpid(child_pids[i], &returned_value, WNOHANG);

        if(WEXITSTATUS(returned_value) == 1) {
          printf("\nKilling all child processes and closing all frontends fds.\n");

          // End all child processes
          for(int j=0; j <= num_connections; j++) {
            close(frontend_fds[j]); // close client file descriptor
            kill(child_pids[j], SIGKILL); // kill child process
          }
          num_connections = 0;

          // Now close parent file descriptor
          close(sockfd);
          return 0;
        }

        printf("PID %d returned %d\n", child_pids[i], WEXITSTATUS(returned_value));

      }

      num_connections = num_connections + 1; // increment connections
      printf("Number of connections: %d\n", num_connections);

    } else {

      printf("\nError forking or too many connections.\n");

    }

  }

  printf("end of main\n");
  return 0;

}

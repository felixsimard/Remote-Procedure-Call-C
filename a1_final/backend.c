/*

COMP 310 / ECSE 427 - Operating Systems
Assignment 1
Felix Simard (260865674)

*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "helper.h" // our custom helper functions for parsing inputs and other

#include "rpc.h" // our RPC methods


#define BUFSIZE 1024
#define NUM_CONNECTIONS_ALLOWED 5
#define SHARED_FILE "pids.txt"

/*--- MAIN ---*/
int main(int argc, char *argv[]) {

  // Variables initialization
  int sockfd, clientfd;
  int accept_connection_pid, pid, returned_value;

  // Lists to hold client file descriptors and children process IDs
  int child_pids[NUM_CONNECTIONS_ALLOWED];
  int frontend_fds[NUM_CONNECTIONS_ALLOWED];

  // Shared file pointer
  FILE *shared_file;

  // Create shared file and close it for now
  shared_file = fopen(SHARED_FILE,"w+");
  fclose(shared_file);

  // Command line arguments
  char host[BUFSIZE];
  int port;


  char line[BUFSIZE];
  char flag;
  const char* server_refused_msg = "n";
  const char* server_accepted_msg = "y";

  if(argc < 3) { // if host, port not provided, use default in helper.h
    strcpy(host, HOST);
    port = PORT;
  } else {
    sscanf(argv[1], "%s", host);
    sscanf(argv[2], "%d", &port);
  }
  //


  // Create server with HOST and PORT (see helper.h)
  if (RPC_Init(host, port, &sockfd) < 0) {
    perror("Error creating server\n");
    return -1;
  }
  //


  shared_file = fopen(SHARED_FILE, "w"); // open for appending

  // Init shared file running flag to 1 (1 = running, 0 = stop)
  fprintf(shared_file, "%d %d ", 1, getpid()); // separated by spaces
  fclose(shared_file);

  while(1) {
    // Accept connections from frontend clients
    if (RPC_Accept(sockfd, &clientfd) < 0) {
      perror("Error accepting connection.\n");
      break;
    }

    // Now check if we can accept further connections

    // Continuously check whether running flag in shared file is 1 (keep running or 0 halt)
    shared_file = fopen(SHARED_FILE, "r");

    flag = fgetc(shared_file); // fetch first character

    if(flag == '0') { // QUIT REQUESTED

      // Close shared file
      fclose(shared_file);

      // Tell frontend server is shutdown
      RPC_Call(clientfd, server_refused_msg, strlen(server_refused_msg));

      // Refuse the incoming connection request
      close(clientfd);

      continue;

    }

    // Close shared file
    fclose(shared_file);

    printf("\nConnection received. Client FD: %d\n", clientfd);

    // Tell frontend connection has been received
    RPC_Call(clientfd, server_accepted_msg, strlen(server_accepted_msg));


    if((clientfd - 3) > NUM_CONNECTIONS_ALLOWED) {
      printf("\nToo many connections.\n");
      close(clientfd);
      continue;
    }

    pid = fork();
    if(pid == 0) {

      // Append to shared PIDs file
      int child_pid = getpid();
      printf("Passing PID %d\n", child_pid);

      // Write PID to shared file
      shared_file = fopen(SHARED_FILE, "a");
      fprintf(shared_file, "%d ", child_pid);
      fclose(shared_file);

      // Serve the cient process
      RPCServ(clientfd);

    } else if(pid < 0) { // parent

      printf("\nError forking.\n");

    }

  }



  printf("\nBackend terminated.\n\n");
  return 0;

}

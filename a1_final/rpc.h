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
int RPC_Init(const char *host, uint16_t port, int *sockfd);

/*--- CLOSE SERVER ---*/
int RPC_Close(int sockfd);

/*--- CONNECT TO SERVER ---*/
int RPC_Connect(const char *host, uint16_t port, int *sockfd);

/*--- ACCEPT CONNECTIONS ---*/
int RPC_Accept(int sockfd, int *clientfd);

/*--- SEND MESSAGE ---*/
ssize_t RPC_Call(int sockfd, const char *buf, size_t len);

/*--- RECEIVE MESSAGE ---*/
ssize_t RPC_Receive(int sockfd, char *buf, size_t len);


/*--------- CALCULATOR METHODS ----------*/

/*--- addInts a b ---*/
int add(int a, int b);

/*--- multiplyInts a b ---*/
int multiply(int a, int b);

/*--- divideFloats a b ---*/
float divide(float a, float b);

/*--- sleep x ---*/
int mySleep(int x);

/*--- factorial x ---*/
uint64_t factorial(int x);

/*----------------------------------------*/


/*--- SERVE CLIENT ---*/
int RPCServ(int clientfd);

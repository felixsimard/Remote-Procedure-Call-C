/*

COMP 310 / ECSE 427 - Operating Systems
Assignment 1
Felix Simard (260865674)

*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "helper.h" // our custom helper functions for parsing inputs and other

#define BUFSIZE 1024
#define NUM_CONNECTIONS_ALLOWED 5
#define SHARED_FILE "pids.txt"

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

/*--- MAIN ---*/
int main(int argc, char *argv[]);

/*

COMP 310 / ECSE 427 - Operating Systems
Assignment 1
Felix Simard (260865674)

*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

#define HOST "0.0.0.0"
#define PORT 11657

/*----------- Helper functions -----------*/
char** parseMessage(char* msg);
/*

 Helper function to parse user input into array of elements containing:

 array[0] -> the action to be performed (ie: add, multiply, divide, etc)
 array[1] -> parameter 1
 array[2] -> parameter 2


*/

char* cleanInput(char* msg);
/*

Helper function to clean the input of trailing carriage returns.

*/

/*----------------------------------------*/

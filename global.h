/***********************************************************
* Author: Jasmine Mittal
* Email: mittalj@oregonstate.edu
* Date Created: 8/9/20
* Filename: global.h is the header file that contains 
* the thread functions
************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

#ifndef __GLOBAL_H
#define __GLOBAL_H

#define SIZE 1000

void *inputThread(void *args);
void *lineSeparatorThread(void *args);
void *plusSignsThread(void *args);
void *outputThread(void *args);
void processLineSep(char contentArr[]);
char* processPlusSign(char contentArray[]);

#endif
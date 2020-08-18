/***********************************************************
* Author: Jasmine Mittal
* Email: mittalj@oregonstate.edu
* Date Created: 8/9/20
* Filename: main.c is the execution file that creates and 
* joins the threads 
************************************************************/
#include "global.h"

int main(int argc, char *argv[]) {

    // Declare threads for the producer-consumer pipelines
    pthread_t t1, t2, t3, t4;

    // Create all four threads 
    pthread_create(&t1, NULL, inputThread, NULL);
    pthread_create(&t2, NULL, lineSeparatorThread, NULL);
    pthread_create(&t3, NULL, plusSignsThread, NULL);
    pthread_create(&t4, NULL, outputThread, NULL);
    
    // Join all the threads 
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);

    return 0;
}
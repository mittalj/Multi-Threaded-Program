/***********************************************************
* Author: Jasmine Mittal
* Email: mittalj@oregonstate.edu
* Date Created: 8/9/20
* Filename: global.c is the function implementation file
* that contains the function declarations 
************************************************************/
#include "global.h"

/***** Declares variables for first producer-consumer interactions *****/
char buffer_one[SIZE];
int characters;
int isBuffOneFull = 0;

// Initialize the mutex
pthread_mutex_t mutex_one = PTHREAD_MUTEX_INITIALIZER;

// Initialize the condition variables
pthread_cond_t full_one = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty_one = PTHREAD_COND_INITIALIZER;

/***** Declares variables for second producer-consumer interactions *****/
char buffer_two[SIZE];
int isBuffTwoFull = 0;

// Initialize the mutex
pthread_mutex_t mutex_two = PTHREAD_MUTEX_INITIALIZER;

// Initialize the condition variables
pthread_cond_t full_two = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty_two = PTHREAD_COND_INITIALIZER;

/***** Declares variables for third producer-consumer interactions *****/
char buffer_three[SIZE];
int isBuffThreeFull = 0;

// Initialize the mutex
pthread_mutex_t mutex_three = PTHREAD_MUTEX_INITIALIZER;

// Initialize the condition variables
pthread_cond_t full_three = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty_three = PTHREAD_COND_INITIALIZER;

// Static buffer output thread 4 to hold remaining characters that still need to be outputed  
static char bufferOutput[10000] = { '\0' };

/***********************************************************
**                    Input Thread One                    **
** This thread takes in the input from stdin and adds it  **
** to Buffer One.                                         **
************************************************************/
void *inputThread(void *args)
{
    char* inputStr = NULL;
    int is_more_inputData = 1;
    size_t len = 0;

    // Loop to keep reading data from stdin until terminating "DONE" is read
    while (is_more_inputData != 0) {

        // Get input line data from file or terminal
        characters = getline(&inputStr,&len,stdin);

        // Check if the line contains the word "DONE". If it does, set the flag to 0 for is_more_inputData
         if (characters == -1 || (characters == 5) && strncmp(inputStr, "DONE", 4) == 0){
            is_more_inputData = 0;
        }

        // Lock mutex one
        pthread_mutex_lock(&mutex_one);

        // Wait for buffer one to empty out 
        while (isBuffOneFull == 1){
            pthread_cond_wait(&empty_one, &mutex_one);
        }

        // Add input data to buffer one 
        memset(buffer_one, '\0', sizeof(buffer_one));
        strcpy(buffer_one, inputStr);
        isBuffOneFull = 1;

        // Signal to the line sep thread that buffer one is full 
        pthread_cond_signal(&full_one);
    
        // Unlock mutex one 
        pthread_mutex_unlock(&mutex_one);
    }
    free(inputStr);
    return NULL;
}

/***********************************************************
**                   Line Separator Thread 2              **
** This thread consumes buffer one's contents, adds it to **
** memory, processes the data, and adds it to buffer two. **
************************************************************/
void *lineSeparatorThread(void *args)
{
    int end_loop = 0;
    while (1) {
         // Lock mutex two     
        pthread_mutex_lock(&mutex_one);

        // Wait for buffer one to get full
        while (isBuffOneFull == 0){
            pthread_cond_wait(&full_one, &mutex_one);
        }

        // Add buffer one contents into variable buff_one_contents
        char buff_one_contents[1000];
        memset(buff_one_contents, '\0', sizeof(buff_one_contents));
        strcpy(buff_one_contents, buffer_one);
        isBuffOneFull = 0;

        // Signal to the input thread that buffer one is no longer empty
        pthread_cond_signal(&empty_one);

        // Unlock mutex one
        pthread_mutex_unlock(&mutex_one);

        // Process line separator by removing null terminator 
        processLineSep(buff_one_contents);

        // Check if the line contains the word "DONE". If it does, set the flag to 0 for end_loop
        if ((strlen(buff_one_contents) == 5) && strncmp(buff_one_contents, "DONE", 4) == 0){
           end_loop = 1;
        }

        // Lock mutex two
         pthread_mutex_lock(&mutex_two);

        // Wait for buffer two to get emptied 
        while (isBuffTwoFull == 1){
            pthread_cond_wait(&empty_two, &mutex_two);
        }

        // Copy contents of buff_one_contents into buffer two
        memset(buffer_two, '\0', sizeof(buffer_two));
        strcpy(buffer_two, buff_one_contents);
        isBuffTwoFull = 1;

        // Signal to plus sign thread that buffer two is full
        pthread_cond_signal(&full_two);

        // Unlock mutex two
        pthread_mutex_unlock(&mutex_two);

        // Break out of processing loop if "DONE" was read
        if (end_loop == 1) {
            break;
        }
    }
    return NULL;
}   

/***********************************************************
**                 Line Separator Process Data            **
** Function to replace the newline with a null terminator.**
************************************************************/
void processLineSep(char contentArr[]) {
    int lengthArr = strlen(contentArr);
    if (contentArr[lengthArr-1] == '\n'){
        contentArr[lengthArr-1] = ' ';
    }
}

/***********************************************************
**                   Plus Sign Thread 3                   **
** This thread consumes buffer two's contents, adds it to **
** memory, processes the data, and adds it to buffer 3.   **
************************************************************/
void *plusSignsThread(void *args)
{
    int end_loop = 0;

    while (1) {
        // Lock mutex two         
        pthread_mutex_lock(&mutex_two);

        // Wait for buffer two to get full
        while (isBuffTwoFull == 0){
            pthread_cond_wait(&full_two, &mutex_two);
        }
           
       // Add buffer two contents into variable buff_two_contents
        char buff_two_contents[1000];
        memset(buff_two_contents, '\0', sizeof(buff_two_contents));
        strcpy(buff_two_contents, buffer_two);
        isBuffTwoFull = 0;

        // Signal to line separator thread that buffer two is empty
        pthread_cond_signal(&empty_two);

        // Unlock mutex two
        pthread_mutex_unlock(&mutex_two);

        // Process Data to convert "++" into "^"
        char* updatedArr = NULL;
        updatedArr = processPlusSign(buff_two_contents);

        // Check if the line contains the word "DONE". If it does, set the flag to 0 for end_loop
        if ((strlen(buff_two_contents) == 5) && strncmp(buff_two_contents, "DONE", 4) == 0){
           end_loop = 1;
        }

        // Lock mutex three
         pthread_mutex_lock(&mutex_three);

        // Wait for signal for buffer three to empty out 
        while (isBuffThreeFull == 1){
            pthread_cond_wait(&empty_three, &mutex_three);
        }

        // Copy contents of memory into buffer three
        memset(buffer_three, '\0', sizeof(buffer_three));
        strcpy(buffer_three, updatedArr);
        free(updatedArr);
        isBuffThreeFull = 1;

        // Signal that buffer three is full
        pthread_cond_signal(&full_three);

        // Unlock mutex three
        pthread_mutex_unlock(&mutex_three);

        // Break out of processing loop if "DONE" was read
        if (end_loop == 1) {
            break;
        }
    }
    return NULL;
}

/***********************************************************
**                   Plus Sign Process Data               **
** This function replaces "++" with "^" when it is found. **
** Citation: https://www.geeksforgeeks.org/c-program-     **
** replace-word-text-another-given-word/                  **
************************************************************/
char* processPlusSign(char contentArray[]) 
{
    char* result; 
    int i, cnt = 0; 
    char newW[] = "^";
    char oldW[] = "++";
    int newWlen = strlen(newW); 
    int oldWlen = strlen(oldW); 
    char* strPtr = contentArray;
  
    // Counting the number of times that the old word occurs in the string 
    for (i = 0; strPtr[i] != '\0'; i++) { 
        if (strstr(&strPtr[i], oldW) == &strPtr[i]) { 
            cnt++; 
  
            // Jumping to index after the old word. 
            i += oldWlen - 1; 
        } 
    } 
    // Allocate memory for the new string, taking in account the length of the replaced words 
    result = (char*)malloc(i + cnt * (newWlen - oldWlen) + 1); 
    i = 0; 
    // Loop to replace the old word with the new word 
    while (*strPtr) { 
        // compare the substring with the result 
        if (strstr(strPtr, oldW) == strPtr) { 
            strcpy(&result[i], newW); 
            i += newWlen; 
            strPtr += oldWlen; 
        } 
        else
            result[i++] = *strPtr++; 
    } 
    // End string with a null terminator and return the result 
    result[i] = '\0'; 
    return result; 
} 

/***********************************************************
**                    Output Thead 4                      **

************************************************************/
void *outputThread(void *args)
{
    int end_loop = 0;

    while (1) {
        // Lock mutex three     
        pthread_mutex_lock(&mutex_three);

        // Buffer three is empty. Wait for signal that the buffer three has data
        while (isBuffThreeFull == 0){
            pthread_cond_wait(&full_three, &mutex_three);
        }
           
       // Add buffer three contents into variable buff_three_contents
        char buff_three_contents[1000];
        memset(buff_three_contents, '\0', sizeof(buff_three_contents));
        strcpy(buff_three_contents, buffer_three);
        isBuffThreeFull = 0;

        // Signal to plus signs thread that buffer three is empty
        pthread_cond_signal(&empty_three);

        // Unlock mutex three
        pthread_mutex_unlock(&mutex_three);

        // Check if the line contains the word "DONE". If it does, set the flag to 0 for end_loop
        if ((strlen(buff_three_contents) == 5) && strncmp(buff_three_contents, "DONE", 4) == 0){
           end_loop = 1;
        }
        
        // Concat string to the bufferOutput if no terminating line
        if (end_loop == 0){
            strcat(bufferOutput, buff_three_contents);
        }

        // Check length of the bufferOutput
        if (strlen(bufferOutput) < 80) {
            // do not output remaining chars
        }
        else {
            // Keep processing single line if it contains at least 80 chars 
            while(strlen(bufferOutput) >= 80) {
                // Print first 80 chars
                fprintf(stdout, "%.80s\n", bufferOutput);
                memset(bufferOutput, '\0', 80*sizeof(char));
                int idx_i = 0;
                int idx_s = 80;
                int count = 0;
                // Shift the string to start at index 0 
                while (bufferOutput[idx_s] != '\0'){
                    bufferOutput[idx_i] = bufferOutput[idx_s];
                    bufferOutput[idx_s] = '\0';
                    idx_i++;
                    idx_s++;
                }
            }
        }

        // Break out of processing loop if "DONE" was read
        if (end_loop == 1) {
            break;
        }
    }
    return NULL;
}
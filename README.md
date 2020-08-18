# Multi-Threaded-Program
This is a C Program of the Producer-Consumer model to implement a character processing program. Four threads are created: The first to receive data input, the second to convert new lines to null terminators, the third to convert "++" chars to "^", and the last one to output the processed data. 

Instructions to run the code: 
1. Load the following files into a directory folder of your choice. 
global.c
global.h
main.c

2. To compile the program, simply type 
"gcc -std=c99 main.c global.c global.h -o line_processor -lpthread" 
into the command prompt.

3. To run the program, type "./line_processor" into the command prompt.
For file redirection, you may run the following variations:
./line_processor < input1.txt
./line_processor > output1.txt
./line_processor < input1.txt > output1.txt

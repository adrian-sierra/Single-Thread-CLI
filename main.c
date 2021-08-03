/*
 * main.c
 *
 *  Created on: Apr 11, 2021
 *  	Author: adrian
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include "command.h"
#include "string_parser.h"

void interactive_mode(){

    command_line large_token_buffer; // initialize command_line struct for stdin
    command_line small_token_buffer;

    size_t len = 128;
    char *line = malloc(len);

    FILE *file;

    int valid = 0;

    printf(">>> ");

    while (getline(&line, &len, stdin) != -1){ // while loop to read through user input line

   	 large_token_buffer = str_filler(line, ";"); // use str_filler function to tokenize based on ";" delimiter

   	 for(int i = 0; large_token_buffer.command_list[i] != NULL; i++){ // loop through the sections that are separated by ";"

   		 small_token_buffer = str_filler(large_token_buffer.command_list[i], " "); // use str_filler to tokenize based on whitespace

   		 for(int j = 0; small_token_buffer.command_list[j] != NULL; j++){ // loop through whitespace tokenized string
   			 // the following statements simply compare the tokenized string to the commands expected,
   			 // keeping track on if a valid command or "exit" has been entered

   			 // also includes corresponding error handling depending on amount of parameters, or lack of,
   			 // for each corresponding command

   			 // finally the appropriate function call is made for corresponding command

   			 if (strcmp(small_token_buffer.command_list[j], "exit") == 0){
   				 exit(0);
   				 valid = 1;
   			 }

   			 if (strcmp(small_token_buffer.command_list[j], "mkdir") == 0){
   				 valid = 1;
   				 if ( (small_token_buffer.command_list[j+2] != NULL) &&  (strcmp(small_token_buffer.command_list[j+2], "mkdir") == 0)){
   					 printf("Error! Incorrect syntax. No control code found.\n");
   					 break;
   				 }
   				 if (small_token_buffer.command_list[j+2] != NULL){
   					 printf("Error! mkdir must have 1 parameter\n");
   					 break;
   				 }
   				 makeDir(small_token_buffer.command_list[j+1]);
   			 }

   			 if (strcmp(small_token_buffer.command_list[j], "cd") == 0){
   				 valid = 1;
   				 if ( (small_token_buffer.command_list[j+2] != NULL) &&  (strcmp(small_token_buffer.command_list[j+2], "cd") == 0)){
   					 printf("Error! Incorrect syntax. No control code found.\n");
   					 break;
   				 }
   				 if (small_token_buffer.command_list[j+2] != NULL){
   					 printf("Error! cd must have 1 parameter\n");
   					 break;
   				 }
   				 changeDir(small_token_buffer.command_list[j+1]);
   			 }

   			 if (strcmp(small_token_buffer.command_list[j], "rm") == 0){
   				 valid = 1;
   				 if ( (small_token_buffer.command_list[j+2] != NULL) &&  (strcmp(small_token_buffer.command_list[j+2], "rm") == 0)){
   					 printf("Error! Incorrect syntax. No control code found.\n");
   					 break;
   				 }
   				 if (small_token_buffer.command_list[j+2] != NULL){
   					 printf("Error! rm must have 1 parameter\n");
   					 break;
   				 }
   				 if ((file = fopen(small_token_buffer.command_list[j+1], "r"))){
   					 deleteFile(small_token_buffer.command_list[j+1]);
   					 fclose(file);
   				 }
   				 else{
   					 printf("File doesn't exist!\n");
   					 break;
   				 }
   			 }

   			 if (strcmp(small_token_buffer.command_list[j], "cat") == 0){
   				 valid = 1;
   				 if ( (small_token_buffer.command_list[j+2] != NULL) &&  (strcmp(small_token_buffer.command_list[j+2], "cat") == 0)){
   					 printf("Error! Incorrect syntax. No control code found.\n");
   					 break;
   				 }
   				 if (small_token_buffer.command_list[j+2] != NULL){
   					 printf("Error! cat must have 1 parameter\n");
   					 break;
   				 }
   				 if ((file = fopen(small_token_buffer.command_list[j+1], "r"))){
   					 displayFile(small_token_buffer.command_list[j+1]);
   					 fclose(file);
   				 }
   				 else{
   					 printf("File doesn't exist!\n");
   					 break;
   				 }
   			 }

   			 if (strcmp(small_token_buffer.command_list[j], "cp") == 0){
   				 valid = 1;
   				 if ( (small_token_buffer.command_list[j+1] != NULL) &&  (strcmp(small_token_buffer.command_list[j+1], "cp") == 0)){
   					 printf("Error! Incorrect syntax. No control code found.\n");
   					 break;
   				 }
   				 if (small_token_buffer.command_list[j+1] == NULL){
   					 printf("Error! cp must have 2 parameters\n");
   					 break;
   				 }
   				 if (small_token_buffer.command_list[j+2] == NULL){
   					 printf("Error! cp must have 2 parameters\n");
   					 break;
   				 }
   				 if (small_token_buffer.command_list[j+3] != NULL){
   					 printf("Error! Command cp must have 2 parameters\n");
   					 break;
   				 }
   				 if ((file = fopen(small_token_buffer.command_list[j+1], "r"))) {
   					 copyFile(small_token_buffer.command_list[j+1], small_token_buffer.command_list[j+2]);
   					 fclose(file);
   				 }
   				 else{
   					 printf("File doesn't exist!\n");
   					 break;
   				 }
   			 }

   			 if (strcmp(small_token_buffer.command_list[j], "mv") == 0){
   				 valid = 1;
   				 if ( (small_token_buffer.command_list[j+1] != NULL) &&  (strcmp(small_token_buffer.command_list[j+1], "mv") == 0)){
   					 printf("Error! Incorrect syntax. No control code found.\n");
   					 break;
   				 }
   				 if (small_token_buffer.command_list[j+1] == NULL){
   					 printf("Error! mv must have 2 parameters\n");
   					 break;
   				 }
   				 if (small_token_buffer.command_list[j+2] == NULL){
   					 printf("Error! mv must have 2 parameters\n");
   					 break;
   				 }
   				 if (small_token_buffer.command_list[j+3] != NULL){
   					 printf("Error! Command mv must have 2 parameters\n");
   					 break;
   				 }
   				 if ((file = fopen(small_token_buffer.command_list[j+1], "r"))) {
   					 moveFile(small_token_buffer.command_list[j+1], small_token_buffer.command_list[j+2]);
   					 fclose(file);
   				 }
   				 else{
   					 printf("File doesn't exist!\n");
   					 break;
   				 }
   			 }

   			 if (strcmp(small_token_buffer.command_list[j], "ls") == 0){
   				 valid = 1;
   				 if ( (small_token_buffer.command_list[j+1] != NULL) &&  (strcmp(small_token_buffer.command_list[j+1], "ls") == 0)){
   					 printf("Error! Incorrect syntax. No control code found.\n");
   					 break;
   				 }
   				 if (small_token_buffer.command_list[j+1] != NULL){
   					 printf("Error! Unsupported parameters for command: ls\n");
   					 break;
   				 }
   				 listDir();
   			 }

   			 if (strcmp(small_token_buffer.command_list[j], "pwd") == 0){
   				 valid = 1;
   				 if ( (small_token_buffer.command_list[j+1] != NULL) &&  (strcmp(small_token_buffer.command_list[j+1], "pwd") == 0)){
   					 printf("Error! Incorrect syntax. No control code found.\n");
   					 break;
   				 }
   				 if (small_token_buffer.command_list[j+1] != NULL){
   					 printf("Error! Unsupported parameters for command: pwd\n");
   					 break;
   				 }
   				 showCurrentDir();
   			 }
   		 }
   		 if(valid != 1) // handling valid command flag
   			 printf("Error! Unrecognized command: %s\n", large_token_buffer.command_list[i]);

   		 valid = 0;

   		 free_command_line(&small_token_buffer); // freeing the two structs for line buffers
   		 memset(&small_token_buffer, 0, 0);
   	 }
   	 free_command_line(&large_token_buffer);
   	 memset(&large_token_buffer, 0, 0);

   	 printf(">>> ");
    }
    free(line);
}

void file_mode(int argc, char **argv){
    // error handling for appropriate amount of flags and arguments for file mode
    if ( argc <= 2 && strcmp(argv[1], "-f") == 0 )
   	 printf("Missing file\n");
    else if (argc <= 2 && strcmp(argv[1], "-f") != 0 )
   	 printf("Missing file flag\n");
    else{

   	 command_line large_token_buffer;
   	 command_line small_token_buffer;

   	 FILE *fp, *inFPtr, *file;
   	 size_t len = 128;
   	 char *line = malloc(len);

   	 inFPtr = fopen(argv[2], "r"); // opening the argument corresponding to input file for file mode
   	 fp = freopen("output.txt", "w", stdout); // redirection of stdout to the "output.txt" file

   	 int valid = 0;

   	 while(getline(&line, &len, inFPtr) != -1){ // loop through the input file
   		 // rest of process is exact same as interactive mode
   		 large_token_buffer = str_filler(line, ";");

   		 for(int i = 0; large_token_buffer.command_list[i] != NULL; i++){

   			 small_token_buffer = str_filler(large_token_buffer.command_list[i], " ");

   			 for(int j = 0; small_token_buffer.command_list[j] != NULL; j++){

   				 if (strcmp(small_token_buffer.command_list[j], "exit") == 0){
   					 exit(0);
   					 valid = 1;
   				 }

   				 if (strcmp(small_token_buffer.command_list[j], "mkdir") == 0){
   					 valid = 1;
   					 if ( (small_token_buffer.command_list[j+2] != NULL) &&  (strcmp(small_token_buffer.command_list[j+2], "mkdir") == 0)){
   						 printf("Error! Incorrect syntax. No control code found.\n");
   						 fflush(stdout);
   						 break;
   					 }
   					 if (small_token_buffer.command_list[j+2] != NULL){
   						 printf("Error! mkdir must have 1 parameter\n");
   						 fflush(stdout);
   						 break;
   					 }
   					 makeDir(small_token_buffer.command_list[j+1]);
   					 fflush(stdout);
   				 }

   				 if (strcmp(small_token_buffer.command_list[j], "cd") == 0){
   					 valid = 1;
   					 if ( (small_token_buffer.command_list[j+2] != NULL) &&  (strcmp(small_token_buffer.command_list[j+2], "cd") == 0)){
   						 printf("Error! Incorrect syntax. No control code found.\n");
   						 fflush(stdout);
   						 break;
   					 }
   					 if (small_token_buffer.command_list[j+2] != NULL){
   						 printf("Error! cd must have 1 parameter\n");
   						 fflush(stdout);
   						 break;
   					 }
   					 changeDir(small_token_buffer.command_list[j+1]);
   					 fflush(stdout);
   				 }

   				 if (strcmp(small_token_buffer.command_list[j], "rm") == 0){
   					 valid = 1;
   					 if ( (small_token_buffer.command_list[j+2] != NULL) &&  (strcmp(small_token_buffer.command_list[j+2], "rm") == 0)){
   						 printf("Error! Incorrect syntax. No control code found.\n");
   						 fflush(stdout);
   						 break;
   					 }
   					 if (small_token_buffer.command_list[j+2] != NULL){
   						 printf("Error! rm must have 1 parameter\n");
   						 fflush(stdout);
   						 break;
   					 }
   					 if ((file = fopen(small_token_buffer.command_list[j+1], "r"))){
   						 deleteFile(small_token_buffer.command_list[j+1]);
   						 fclose(file);
   					 }
   					 else{
   						 printf("File doesn't exist!\n");
   						 fflush(stdout);
   						 break;
   					 }
   				 }

   				 if (strcmp(small_token_buffer.command_list[j], "cat") == 0){
   					 valid = 1;
   					 if ( (small_token_buffer.command_list[j+2] != NULL) &&  (strcmp(small_token_buffer.command_list[j+2], "cat") == 0)){
   						 printf("Error! Incorrect syntax. No control code found.\n");
   						 fflush(stdout);
   						 break;
   					 }
   					 if (small_token_buffer.command_list[j+2] != NULL){
   						 printf("Error! cat must have 1 parameter\n");
   						 fflush(stdout);
   						 break;
   					 }
   					 if ((file = fopen(small_token_buffer.command_list[j+1], "r"))){
   						 displayFile(small_token_buffer.command_list[j+1]);
   						 fflush(stdout);
   						 fclose(file);
   					 }
   					 else{
   						 printf("File doesn't exist!\n");
   						 fflush(stdout);
   						 break;
   					 }
   				 }

   				 if (strcmp(small_token_buffer.command_list[j], "cp") == 0){
   					 valid = 1;
   					 if ( (small_token_buffer.command_list[j+1] != NULL) &&  (strcmp(small_token_buffer.command_list[j+1], "cp") == 0)){
   						 printf("Error! Incorrect syntax. No control code found.\n");
   						 fflush(stdout);
   						 break;
   					 }
   					 if (small_token_buffer.command_list[j+1] == NULL){
   						 printf("Error! cp must have 2 parameters\n");
   						 fflush(stdout);
   						 break;
   					 }
   					 if (small_token_buffer.command_list[j+2] == NULL){
   						 printf("Error! cp must have 2 parameters\n");
   						 fflush(stdout);
   						 break;
   					 }
   					 if (small_token_buffer.command_list[j+3] != NULL){
   						 printf("Error! Command cp must have 2 parameters\n");
   						 fflush(stdout);
   						 break;
   					 }
   					 if ((file = fopen(small_token_buffer.command_list[j+1], "r"))) {
   						 copyFile(small_token_buffer.command_list[j+1], small_token_buffer.command_list[j+2]);
   						 fclose(file);
   					 }
   					 else{
   						 printf("File doesn't exist!\n");
   						 fflush(stdout);
   						 break;
   					 }
   				 }

   				 if (strcmp(small_token_buffer.command_list[j], "mv") == 0){
   					 valid = 1;
   					 if ( (small_token_buffer.command_list[j+1] != NULL) &&  (strcmp(small_token_buffer.command_list[j+1], "mv") == 0)){
   						 printf("Error! Incorrect syntax. No control code found.\n");
   						 fflush(stdout);
   						 break;
   					 }
   					 if (small_token_buffer.command_list[j+1] == NULL){
   						 printf("Error! mv must have 2 parameters\n");
   						 fflush(stdout);
   						 break;
   					 }
   					 if (small_token_buffer.command_list[j+2] == NULL){
   						 printf("Error! mv must have 2 parameters\n");
   						 fflush(stdout);
   						 break;
   					 }
   					 if (small_token_buffer.command_list[j+3] != NULL){
   						 printf("Error! Command mv must have 2 parameters\n");
   						 fflush(stdout);
   						 break;
   					 }
   					 if ((file = fopen(small_token_buffer.command_list[j+1], "r"))) {
   						 moveFile(small_token_buffer.command_list[j+1], small_token_buffer.command_list[j+2]);
   						 fclose(file);
   					 }
   					 else{
   						 printf("File doesn't exist!\n");
   						 fflush(stdout);
   						 break;
   					 }
   				 }

   				 if (strcmp(small_token_buffer.command_list[j], "ls") == 0){
   					 valid = 1;
   					 if ( (small_token_buffer.command_list[j+1] != NULL) &&  (strcmp(small_token_buffer.command_list[j+1], "ls") == 0)){
   						 printf("Error! Incorrect syntax. No control code found.\n");
   						 fflush(stdout);
   						 break;
   					 }
   					 if (small_token_buffer.command_list[j+1] != NULL){
   						 printf("Error! Unsupported parameters for command: ls\n");
   						 fflush(stdout);
   						 break;
   					 }
   					 listDir();
   					 fflush(stdout);
   				 }

   				 if (strcmp(small_token_buffer.command_list[j], "pwd") == 0){
   					 valid = 1;
   					 if ( (small_token_buffer.command_list[j+1] != NULL) &&  (strcmp(small_token_buffer.command_list[j+1], "pwd") == 0)){
   						 printf("Error! Incorrect syntax. No control code found.\n");
   						 fflush(stdout);
   						 break;
   					 }
   					 if (small_token_buffer.command_list[j+1] != NULL){
   						 printf("Error! Unsupported parameters for command: pwd\n");
   						 fflush(stdout);
   						 break;
   					 }
   					 showCurrentDir();
   					 fflush(stdout);
   				 }
   			 }
   			 if(valid != 1){
   				 printf("Error! Unrecognized command: %s\n", large_token_buffer.command_list[i]);
   				 fflush(stdout);
   			 }
   			 valid = 0;

   			 free_command_line(&small_token_buffer);
   			 memset(&small_token_buffer, 0, 0);
   		 }
   		 free_command_line(&large_token_buffer);
   		 memset(&large_token_buffer, 0, 0);
   	 }
   	 free(line);
    }
}

int main(int argc, char **argv){
    // checking main arguments to determine if user wants file mode or interactive mode
    if (argc <= 1)
   	 interactive_mode();
    else
   	 file_mode(argc, argv);

    return 0;
}




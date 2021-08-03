/*
 * command.c
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

void listDir(){
    // utilizes system calls getcwd and write as well as the DIR struct
    // from the dirent.h library that provides functionality to open a directory and
    // and read from that directory
    size_t len = 128;
    char *cwd = malloc(len);

    DIR *dir;
    struct dirent *dp;

    getcwd(cwd, len);
    dir = opendir(cwd);

    while((dp = readdir(dir)) != NULL){
   	 write(1, dp->d_name, strlen(dp->d_name));
   	 write(1, " ", 1);
    }

    write(1, "\n", 1);

    closedir(dir);

    free(cwd);
}

void showCurrentDir(){
    // utilizes getcwd system call to simply get the current working
    // directory and store it in a buffer, and writes that buffer to stdout
    // using write system call
    size_t len = 128;
    char *cwd = malloc(len);

    getcwd(cwd, len);

    write(1, cwd, strlen(cwd));
    write(1, "\n", 1);

    free(cwd);
}

void makeDir(char *dirName){
    // used DIR struct again to error handle a directory already existing
    // and if it does not, we use the mkdir system call to create the
    // appropriate directory named dirName, from arguments
    DIR *dir = opendir(dirName);
    if (dir)
   	 write(1, "Directory already exists!\n", strlen("Directory already exists!\n"));
    else if (ENOENT == errno)
   	 mkdir(dirName, 0777);
    closedir(dir);
}

void changeDir(char *dirName){
    // same process as makeDir function, but use chdir system call here
    DIR *dir = opendir(dirName);
    if (dir){
   	 chdir(dirName);
   	 closedir(dir);
    }
    else if (ENOENT == errno)
   	 write(1, "No such directory exists!\n", strlen("No such directory exists!\n"));
}

void copyFile(char *sourcePath, char *destinationPath){
    size_t len = 128;
    char *cwd = malloc(sizeof(char) * len);
    getcwd(cwd, len); // use getcwd system call in order to have knowledge of what the current directory is

	char *buf2 = strtok(sourcePath, "\n"); // eliminating the newline character

	char *s = strdup(buf2);
	char *token;
	char *rest = s;
	char *filename = malloc(sizeof(char) * len);

	while ((token = strtok_r(rest, "/", &rest))) // tokenize through sourcePath in order to get the file name
	{
   	 if (token != NULL)
   		 strcpy(filename, token);
	}

    char *line =  malloc(sizeof(char) * len);

    int fd, rd, td, is_dir;
    fd = open(sourcePath, O_RDONLY); // open system call to be able to read the corresponding source file to be copied

    struct stat path_stat = {0,}; // this stat struct provides knowledge of whether a given path is a directory or not
    stat(destinationPath, &path_stat);
    is_dir = S_ISDIR(path_stat.st_mode);

    if (is_dir){ // if the destinationPath is a directory, and not a file,
   			  // we will have to change into that directory and name this copied
   			  // file the file name discovered using strtok_r

   	 changeDir(destinationPath);

   	 td = creat(filename, 0644); // creat system call to create new file with given permissions

   	 while((rd = read(fd, line, sizeof(line))) > 0) // loop through open source file using read
   													// system call and using write system call to write to created file
   		 write(td, line, rd);

   	 changeDir(cwd);
    }
    else{ // if destinationPath was a file, we simply create the corresponding file and do the same process of reading
   	   // source file and writing to destination file

   	 td = creat(destinationPath, 0644);

   	 while((rd = read(fd, line, sizeof(line))) > 0)
   		 write(td, line, rd);

    }
    // appropriate free calls
	free(s);
	free(filename);
    free(cwd);
    free(line);

    close(fd);
    close(td);
}

void moveFile(char *sourcePath, char *destinationPath){
    // utilize functionality of two other function that do
    // what this function is intended to do
    copyFile(sourcePath, destinationPath);
    deleteFile(sourcePath);
}

void deleteFile(char *filename){
    // call unlink system call given filename argument
    unlink(filename);
}

void displayFile(char *filename){
    // use open, read, and write system call to display a file
    // to stdout by just looping through the opened file as
    // long as there is something to read
    size_t len = 128;
    char *line = malloc(len);

    int fd, rd;
    fd = open(filename, O_RDONLY);

    while((rd = read(fd, line, sizeof(line))) > 0)
   	 write(1, line, rd);

    close(fd);
    free(line);
}


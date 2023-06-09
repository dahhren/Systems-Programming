//-----------------------------------------------------------------------------
//	Client Server Processes
//
//	Name: lab6receive.c
//
//	Written by: Darren Staton Dsilva - April 2023
//
//	Purpose: Receive files and make a copy 
//
//	Usage: ./receive.o
//
//	Description of parameters: -		
//
//	Subroutines/Libraries: requied: See includes
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#define PIPE_BUF 2048
#define SF "serverfifo"

void messageprt(char mbuf[]) 
{
    int i = 0; // index
    printf("Message: "); // print message
    while (mbuf[i] != '\0') // while not end of string 
    {
        putchar(mbuf[i]); // print character
        i++; // increment index
    }
    putchar ('\n'); // print new line
}

void copy(char mbuf[]) // copy file
{
    int i = 0;
    char filename[PIPE_BUF]; // filename
    int outfile; // output file
    char buffer [1]; // buffer 

    if ((outfile = open("copy.txt", O_WRONLY | O_CREAT, 0666)) < 0) // open file
    {
        perror("open"); // call an error
        exit(1); // exit
    }
    
    i = 0;
    lseek(outfile, 0, SEEK_SET); // set offset to 0
    while (mbuf[i] != '\0')
    {
        buffer[0] = mbuf[i]; // put character into buffer
        write(outfile, buffer, 1); // write to file
        i++;
        lseek(outfile, i, SEEK_SET); // set offset to i
    }
    close(outfile); // close file
}

int main (int argc, char *argv[]) 
{
    int fd, done = 0; // file descriptor, done
    char msgbuf[PIPE_BUF]; // message buffer
    if (mkfifo(SF, 0666) == -1) // make fifo
        if (errno != EEXIST) 
        {
            perror("mkfifo"); 
            exit(1);
        }
    if ((fd = open(SF, O_RDWR)) < 0) // open fifo
    {
        perror ("open"); // call an error
        exit(1);
    }
    while (!done) // while not done
    {
        if (read(fd, msgbuf, PIPE_BUF) < 0) // read from fifo
        {
            perror("read"); 
            exit(1);
        }
        copy(msgbuf); // copy file
    }
    close(fd); // close file descriptor
    return 0;
}

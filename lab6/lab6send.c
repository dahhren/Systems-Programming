//-----------------------------------------------------------------------------
//	Client Server Processes
//
//	Name: lab6send.c
//
//	Written by: Darren Staton Dsilva - April 2023
//
//	Purpose: Send a file to the server if the FIFO type exists
//
//	Usage: ./send.o <filename>
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

/* put input message into mbuf[] with '$‘ and padded with spaces */
void messagecrt(char mbuf[], char input[])
{
    int i;
    for (i = 1; i < PIPE_BUF - 1; i++)
        mbuf[i] = ' ';
    mbuf[i] = '\0';
    i = 0;
    while (input[i] != '\0')
    {
        mbuf[i] = input[i];
        i++;
    }
    mbuf[i] = '$';
} /* make_msg */

/* put file binary into mbuf[] */
void fileprep(char mbuf[], char input[])
{
    int i, infile;
    char buffer[1];
    // create empty mbuf[]
    for (i = 1; i < PIPE_BUF - 1; i++)
        mbuf[i] = ' ';
    // mbuf[i] = '\0';

    // read file with open() and put binary into mbuf[]
    if ((infile = open(input, O_RDONLY)) < 0)
    {
        perror(input);
        exit(1);
    }

    i = 0;
    lseek(infile, 0, SEEK_SET); // set file pointer to beginning of file
    while (read(infile, buffer, 1) > 0)
    {
        // add byte to mbuf[]
        mbuf[i] = buffer[0];
        i++;
        // set file pointer to next byte
        lseek(infile, i, SEEK_SET);
    }

    // add EOF to mbuf[]
    mbuf[i] = '\0';

    close(infile);
}

int main(int argc, char *argv[])
{
    int fd, i; // file descriptor
    char msgbuf[PIPE_BUF]; // message buffer
    if (argc < 2) // check for correct number of arguments
    {
        printf("Usage: send-msg msg...\n"); // print usage
        exit(1);
    }
    if ((fd = open(SF, O_WRONLY | O_NONBLOCK)) < 0) // open FIFO
    {
        perror(SF); // call an error
        exit(1); 
    }
    for (i = 1; i < argc; i++) // for each argument
    {
        if (strlen(argv[i]) > PIPE_BUF - 2) // check if its too long
            printf("Too long: %s\n", argv[i]); // print the error
        else
        {
            fileprep(msgbuf, argv[i]); // put file binary into mbuf[]
            write(fd, msgbuf, PIPE_BUF); // write to FIFO
        }
    }
    close(fd);
    return 0;
} /* end main */
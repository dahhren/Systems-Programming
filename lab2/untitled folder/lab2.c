/*======================================================================
|   File Processing and Command line arguments
|
|   Name: lab2.c
|
|   Written by: Darren Dsilva - Feburary 2023
|
|   Purpose: Reverse a Text File and Create a new text file to reverse it back
|
|   Usage:  ./lab2 <input file> <output file>
|
|   Parameters: Input File and Output file.
|
| ========================================================================
*/
#include <stdlib.h> 
#include <stdio.h>
#include <fcntl.h> 
// #include <unistd.h> 
#define BUFSIZE 512 // To define the size of the file

/* Function to copy the contents from one file to another*/
int copyfile(const char *name1, const char *name2)  
{
    int infile, outfile; // Setting up the variables
    ssize_t nread; // Size of the file
    char buffer[BUFSIZE]; // Creating the buffer variable
    if((infile = open(name1, O_RDONLY)) == -1) // Opening Input file
        return -1;
    if((outfile = open(name2, O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1) // Opening Output file
        {  
        close(infile); // Closing the file
        return -2;
        }
    
    nread = lseek(infile, (ssize_t) 0, SEEK_END); //Reading the file
    int i; 
    for (i = -1; i>= -nread; i--) 
    {
        lseek(infile, i, SEEK_END); //Seeks to the end of the file1
        read(infile, buffer, 1); // 
        write(outfile, buffer, 1); 
    }

    close(infile); // Closes the infile
    close(outfile); // Closes the Outfile
    if(nread == -1)
        return -4;
    else
        return 0; 
}

int main(int argc, char *argv[]) // Testing
    {
    char *file1 = argv[1]; //Creating Variables for File1 &
    char *file2 = argv[2]; //File 2
    int result;
    result = copyfile(file1, file2); // copies the reversed content to file2 
    result = copyfile(file2, "file3"); // copies the reversed REVERSED content to file3

    switch(result) //Results of File Processing
        {
        case 0:     printf("Contents copied from %s to %s\n", file1, file2); // Prints if Reverse successful
                    break;
        case -1:    printf("Error opening input file: %s\n", file1); // Prints if there is an error opening file1
                    break;
        case -2:    printf("Error opening output file: %s\n", file2); // Prints if there is an error opening file2
                    break;
        case -3:    printf("Error writing to file: %s\n", file2); // Prints if there is an error writing to file2 
                    break;
        case -4:    printf("Error in last write to file: %s\n", file2); // Prints if there is an error in the last write to file
                    break;
        default:    printf("Unknown error occurred\n");
        } 
    return 0;
}
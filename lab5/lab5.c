//-----------------------------------------------------------------------------
//	Signal Processing
//
//	Name: lab5.c
//
//	Written by: Darren Staton Dsilva - March 2023
//
//	Purpose: To Create a program to recognize and handle signals
//
//	Usage: ./lab5.o 
//
//	Description of parameters:
//		
//
//	Subroutines/Libraries: requied: See includes
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>

int counter = 0; // counter for number of times ^C is pressed
pid_t pid; // process id

void sigint_handler() // function to handle ^C SIGINT
{
    counter++; // increment counter
    printf("\nTimes Received ^C SIGINT: %2d\n", counter); // display number of times ^C is pressed
    if (counter >= 7) // if ^C is pressed 7 times, 
    {
        printf("\nReceived ^C SIGINT %d times. Deactivating SIGINT handler & terminating program!\n", counter); 
        signal(SIGINT, SIG_DFL);  // deactivate handler
        exit(0); // exit program
    }
}

void sig_usr(int signo) // function to handle SIGQUIT and SIGUSR1
/* argument is signal number */
{
    if (signo == SIGINT) // if signal is ^C
    {
        sigint_handler(); // call sigint_handler function
    }
    else if (signo == SIGQUIT) // if signal is ^(backslash)
    {  
        pid = fork(); // create child process
        if (pid == 0)
        {
            printf("I am the child and I am sending a signal\n");
            kill(getppid(), SIGUSR1); // send signal to parent
            exit(1); // exit child process
        }
    }
    else if (signo == SIGUSR1) // if signal is SIGUSR1
    {
        wait(0); // wait for child process to finish and print message
        printf("I am the parent and I received a signal\n");
        exit(2); // exit parent process
    }
    else 
    {
      printf("Error: received signal %d\n",signo); // if signal is not ^C, ^(backslash) or SIGUSR1, display error and exit program
      exit(3); 
    }
    return; 
}

int main() 
    /* handles two signals */
{
    if (signal(SIGINT,sig_usr) == SIG_ERR) 
        printf("Error: Cannot catch ^C SIGINT\n"); // if ^C is pressed, call sig_usr function
    if (signal(SIGQUIT,sig_usr) == SIG_ERR) 
        printf("Error: Cannot catch ^\ SIGQUIT\n"); // if ^(backslash) is pressed, call sig_usr function
    if (signal(SIGUSR1,sig_usr) == SIG_ERR)
        printf("Error: Cannot catch SIGUSR1\n"); // if SIGUSR1 is received, call sig_usr function

    while(counter < 7) //While program is still running, display a message that says waiting for signal
        {
            printf("Waiting for Signal\n");
            pause();
            /* pause until signal handler has processed signal */
        }
    return 0;
}
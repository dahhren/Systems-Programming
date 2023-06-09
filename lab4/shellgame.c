//-----------------------------------------------------------------------------
//	The SHELL Game 
//
//	Name: shellgame.c
//
//	Written by: Darren Staton Dsilva - March 2023
//
//	Purpose: To Create your own shell to recognize your own commands
//
//	Usage: ./shell.o 
//
//	Description of parameters:
//		
//
//	Subroutines/Libraries: requied: See includes
//-----------------------------------------------------------------------------

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/stat.h>

void Users() //function to display the users online
{
    execlp ("w", "w", NULL); //execute the w command
    printf("Execlp Failed\n"); //if the command fails
    exit(1); //exit the program
}

void List() //function to display the files in the current directory
{
    execlp ("ls", "ls", NULL); //execute the ls command
    printf("Execlp Failed\n"); //if the command fails
    exit(1); //exit the program
}

void Date() //function to display the date and time of the system
{
    execlp ("date", "date", NULL); //execute the date command
    printf("Execlp Failed\n"); //if the command fails
    exit(1); //exit the program
}

void JobTree() //function to display the job tree of the user
{
    char *user = getenv("USER");
    //execute the ps command to display processes of the user
    execlp ("ps", "ps", "-o", "user:32,pid,stime,tty,cmd", "-U", user, "--forest", NULL);
    printf("Execlp Failed\n"); //if the command fails
    exit(1); //exit the program
}

void Help() //function to display the help menu
{
    {
        printf("\nHelp Menu\n"); //display the help menu
        //display the commands and their functions
        printf("online - Displays the users online on Loki \n"); 
        printf("info - List Files in the Current Directory \n");
        printf("now - Displays Date and Time of the System \n");
        printf("jobtree - Displays the Job Tree of the User \n");
        printf("logout - Self Explanatory :) \n");
        exit(1); //exit the program
    }
}

int main(int argc, char *argv[])
{
    char i[128]; //array to store the command entered
    char *user; //array to store the argument entered
    int done = 0; //variable to check if the user wants to logout
    pid_t pid; //variable to store the process id

    while (!done) //loop to keep the shell running
    {
    printf("Choose a Process \nTo Display commands, type 'help'\nshell>"); //display the shell prompt
    scanf("%s", i ); //scan the command entered
    if (strcmp (i, "logout") == 0) //check if the user wants to logout
    {
        printf("Logging Out\n" ); //display the logout message
        done = 1; //set the done variable to 1 to exit the loop
    }
    else
        {
            pid = fork(); //create a child process
            if(pid == 0) //check if the child process is created
                {
                    if (strcmp (i, "help") == 0) //check if the user wants to display the help menu
                        {
                            Help(); //call the Help function
                        }
                    else if (strcmp (i, "online") == 0) //check if the user wants to display the users online
                        { 
                            Users(); //call the Users function
                        }
                    else if (strcmp (i, "info") == 0) //check if the user wants to display the files in the current directory
                        {
                            List(); //call the List function
                        }
                    else if (strcmp (i, "now") == 0) //check if the user wants to display the date and time of the system
                        {
                            Date(); //call the Date function
                        }
                    else if (strcmp (i, "jobtree") == 0) //check if the user wants to display the job tree of the user
                        {
                            JobTree(); //call the JobTree function
                        }
                }
            else if (pid > 0) //check if the parent process is created
                { 
                wait((int *)0); //wait for the child process to complete
                printf("Child Complete\n"); //display the child complete message
                } 
        }
    }
}



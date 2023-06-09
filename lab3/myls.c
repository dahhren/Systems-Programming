//-----------------------------------------------------------------------------
//	My Version of ls - to list the files in a specified directory
//
//	Name: myls.c
//
//	Written by: Darren Staton Dsilva - March 2023
//
//	Purpose: List files in a directory like ls -l
//
//	Usage: ./my_ls.out [-o days] [-y days] [-l bytes] [-s bytes] [dir]
//
//	Description of parameters:
//		-o older: list files older than the parameter argument 'days'
//		-y younger: list files younger than the parameter argument 'days'
//		-l larger: list files larger than bytes entered
//		-s smaller: list files smaller than bytes entered
//		dir: list files in that directory, default is current working directory
//
//	Subroutines/Libraries: requied: See includes
//-----------------------------------------------------------------------------

#include<stdio.h>
#include<dirent.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<pwd.h>
#include<grp.h>
#include<unistd.h>
#include<sys/stat.h>
#include<getopt.h>

int fileOlder(int old, time_t fileTime) //function to check if file is older than the number of days entered
{
    old = old * 24 * 60 * 60; //convert days to seconds
    old = time(NULL) - old; //get current time and subtract days
    if(fileTime < old) 
    {
        return 1; //return 1 if file is older than days
    }
    return 0; 
}

int fileYounger(int young, time_t fileTime) //function to check if file is younger than the number of days entered
{
    young = young * 24 * 60 * 60; //convert days to seconds
    young = time(NULL) - young; //get current time and subtract days
    if(fileTime > young)
    {
        return 1; //return 1 if file is younger than days
    }
    return 0;
}

void printFileStats(char *fileName, struct stat fileStat) //function to print the file details
{
    char permissions[10];   //array to store the permissions
    if (S_ISDIR(fileStat.st_mode)) //check if file is a directory
    {
        permissions[0] = 'd'; 
    }
    else if (S_ISLNK(fileStat.st_mode)) //check if file is a link
    {
        permissions[0] = 'l';
    }
    else 
    {    
        permissions[0] = '-';    //if not a directory or link, it is a file
    }

    //check if the user or the group has read, write or exec permissions of the file, and set the character accordingly
    permissions[1] = fileStat.st_mode & S_IRUSR ? 'r' : '-';
    permissions[2] = fileStat.st_mode & S_IWUSR ? 'w' : '-';
    permissions[3] = fileStat.st_mode & S_IXUSR ? 'x' : '-';
    permissions[4] = fileStat.st_mode & S_IRGRP ? 'r' : '-';
    permissions[5] = fileStat.st_mode & S_IWGRP ? 'w' : '-';
    permissions[6] = fileStat.st_mode & S_IXGRP ? 'x' : '-';
    permissions[7] = fileStat.st_mode & S_IROTH ? 'r' : '-';
    permissions[8] = fileStat.st_mode & S_IWOTH ? 'w' : '-';
    permissions[9] = fileStat.st_mode & S_IXOTH ? 'x' : '-';
    permissions[10] = '\0'; //add null character to end of string

    struct group *grp; //get the group name
    grp = getgrgid(fileStat.st_gid);
    char *groupID = grp ? grp->gr_name : "-"; //if group is null, set it to -

    struct passwd *user; //get the user name
    user = getpwuid(fileStat.st_uid); 
    char *userID = user ? user->pw_name : "-"; //if user is null, set it to -

    time_t lastmodifiedtime = fileStat.st_mtime; //get the last modified time
    char *lastmodifiedread = ctime(&lastmodifiedtime); //convert the time to a string
    lastmodifiedread[strlen(lastmodifiedread) - 1] = '\0'; //remove the new line character from the string

    //print the file details
    printf("%7lu %11s %5lu %22s %22s %8li %28s %11s\n",
		   fileStat.st_ino, permissions, fileStat.st_nlink, userID, groupID, fileStat.st_size, lastmodifiedread, fileName);
}

int main(int argc, char *argv[])
{
    //storing the variables
    int c; 
    int old = 0; 
    int young = 0;
    int larger = 0;
    int smaller = 0;
    while ((c = getopt(argc, argv, "o:y:l:s:")) != -1) //get the options from the command line
    {
        switch (c)
        {
            case 'o':
                old = atoi(optarg);
                break;
            case 'y':
                young = atoi(optarg);
                break;
            case 'l':
                larger = atoi(optarg);
                break;
            case 's':
                smaller = atoi(optarg); 
                break;
        }
    }
    
    // directory name from command line
    char *dirName; 
    if (optind < argc)
        {
            dirName = argv[optind]; //get the directory name from the command line
        }
    else
        {
            dirName = "."; //if no directory is entered, use the current working directory
        }

    DIR *dp; //open the directory
    struct dirent *dir; //store directory entry
    if ((dp = opendir(dirName)) == NULL) //if directory cannot be opened, print error and exit
    {
        fprintf( stderr, "Cannot open dir\n"); 
        exit(1);
    }
    //switch the directory to the one entered in command line    
    //print the header 
    chdir(dirName);
    printf("%6s %9s %10s %12s %22s %16s %19s %20s\n", 
            "inode", "Perms", "Links", "UID", "GID", "Size", "Modified", "FileName");
    
    // read entries 
    while ((dir = readdir(dp)) != NULL ) //read the directory entries
    {
        // ignore empty records
        if(dir->d_ino != 0 )
        {
            char *fileName;     //store the file name
            struct stat fileStat; //store the file stats
            fileName = dir->d_name; //get the file name
            stat(fileName, &fileStat); //get the file stats

            if
                (
                    ((old != 0 && fileOlder(old, fileStat.st_mtime) == 1) || old == 0) && //check if the file is older than the number of days entered
                    ((young != 0 && fileYounger(young, fileStat.st_mtime) == 1) || young == 0) && //check if the file is younger than the number of days entered
                    ((larger != 0 && larger, fileStat.st_size > larger) || larger == 0) && //check if the file is larger than the number of bytes entered
                    ((smaller != 0 && smaller, fileStat.st_size < smaller) || smaller == 0) //check if the file is smaller than the number of bytes entered
                )
            {
                    printFileStats(fileName, fileStat); //print the file details
            }
        }
    }
    closedir(dp); //close the directory
    return 0;
} 
// end main
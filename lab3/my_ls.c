/*-----------------------------------------------------------------------------
	List files in a directory

	Name: my_ls.c

	Written by: Adyan Ehtesham - March 2023

	Purpose: List files in a directory like ls -l

	Usage: ./my_ls.out [-o days] [-y days] [-l bytes] [-s bytes] [dir]

	Description of parameters:
		-o older: list files older than the parameter argument 'days'
		-y younger: list files younger than the parameter argument 'days'
		-l larger: list files larger than bytes entered
		-s smaller: list files smaller than bytes entered
		dir: list files in that directory, default is current working directory

	Subroutines/Libraries: requied: See includes
 *---------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

/// @brief Check if file is older than the 'older' number of days
/// @param older number of days file is checked against
/// @param fileTime file modification time
/// @return 1 if file is older, 0 if not
int fileIsOlder(int older, time_t fileTime)
{
	older = older * 24 * 60 * 60; // convert days to seconds
	older = time(NULL) - older;	  // get current time and subtract days
	if (fileTime < older)		  // check if file is older than days
	{
		return 1;
	}
	return 0;
}

/// @brief Check if file is younger than the 'younger' number of days
/// @param older number of days file is checked against
/// @param fileTime file modification time
/// @return 1 if file is younger, 0 if not
int fileIsYounger(int younger, time_t fileTime)
{
	younger = younger * 24 * 60 * 60; // convert days to seconds
	younger = time(NULL) - younger;	  // get current time and subtract days
	if (fileTime > younger)			  // check if file is younger than days
	{
		return 1;
	}
	return 0;
}

/// @brief get group name from group id
/// @param filestat struct stat of file
/// @return group name
const char *getFileGroupName(struct stat filestat)
{
	struct group *grp;
	grp = getgrgid(filestat.st_gid);		// get group name from group id
	char *group = grp ? grp->gr_name : "-"; // if group name is null, set to '-'

	return group;
}

/// @brief get user name from user id
/// @param filestat struct stat of file
/// @return user name
const char *getFileUserName(struct stat filestat)
{
	struct passwd *usr;
	usr = getpwuid(filestat.st_uid);	   // get user name from user id
	char *user = usr ? usr->pw_name : "-"; // if user name is null, set to '-'
	return user;
}

/// @brief get last modified time and convert to readable format
/// @param filestat struct stat of file
/// @return last modified time in readable format
const char *getReadableLastModTime(struct stat filestat)
{
	// get last modified time and convert to readable format
	time_t last_modified_time = filestat.st_mtime;					   // get last modified time
	char *last_modified_readable = ctime(&last_modified_time);		   // convert to readable format
	last_modified_readable[strlen(last_modified_readable) - 1] = '\0'; // remove newline character

	return last_modified_readable;
}

/// @brief Prints file details, mimics ls -li
/// @param filename name of file
/// @param filestat struct containing file details
void printFileStats(char *filename, struct stat filestat)
{
	// tried getting permissions using a function but it didn't work
	// something to do with the permissions beein a pointer to a char array
	// get permissions string
	char permissions[10];		   // 10 characters for permissions
	if (S_ISDIR(filestat.st_mode)) // check if file is a directory
		permissions[0] = 'd';
	else if (S_ISLNK(filestat.st_mode)) // check if file is a link
		permissions[0] = 'l';
	else
		permissions[0] = '-'; // otherwise it is a regular file

	// check if user, group, or other has read, write, or execute permissions
	// and set the corresponding character in the permissions string
	permissions[1] = filestat.st_mode & S_IRUSR ? 'r' : '-';
	permissions[2] = filestat.st_mode & S_IWUSR ? 'w' : '-';
	permissions[3] = filestat.st_mode & S_IXUSR ? 'x' : '-';
	permissions[4] = filestat.st_mode & S_IRGRP ? 'r' : '-';
	permissions[5] = filestat.st_mode & S_IWGRP ? 'w' : '-';
	permissions[6] = filestat.st_mode & S_IXGRP ? 'x' : '-';
	permissions[7] = filestat.st_mode & S_IROTH ? 'r' : '-';
	permissions[8] = filestat.st_mode & S_IWOTH ? 'w' : '-';
	permissions[9] = filestat.st_mode & S_IXOTH ? 'x' : '-';
	permissions[10] = '\0'; // null terminate the string

	// get the user and group names
	char *user = getFileUserName(filestat);
	char *group = getFileGroupName(filestat);

	char *last_modified_readable = getReadableLastModTime(filestat);

	// print file stats
	printf("%7lu %11s %5lu %20s %20s %8li %24s %s\n",
		   filestat.st_ino, permissions, filestat.st_nlink, user, group, filestat.st_size,
		   last_modified_readable, filename);
}

/// @brief Prints file details, mimics ls -li
int main(int argc, char *argv[])
{
	int c;			 // used for getopt, holds option character
	int older = 0;	 // number of days file is older than
	int younger = 0; // number of days file is younger than
	int larger = 0;	 // file size is larger than
	int smaller = 0; // file size is smaller than
	// get command line options using getopt and store in variables
	while ((c = getopt(argc, argv, "o:y:l:s:")) != -1)
	{
		switch (c)
		{
		case 'o':
			older = atoi(optarg);
			break;
		case 'y':
			younger = atoi(optarg);
			break;
		case 'l':
			larger = atoi(optarg);
			break;
		case 's':
			smaller = atoi(optarg);
			break;
		}
	}

	// get directory name from command line, if none given, use current directory
	char *dir_name;
	if (optind < argc)
	{
		dir_name = argv[optind];
	}
	else
	{
		dir_name = ".";
	}

	// open directory
	DIR *dp;
	struct dirent *dir; // store directory entries
	if ((dp = opendir(dir_name)) == NULL)
	{
		// if directory cannot be opened, print error and exit
		fprintf(stderr, "Cannot open dir\n");
		exit(1);
	}

	// change directory to directory provided
	// so that context is correct when printing file stats
	chdir(dir_name);

	// print header
	printf("%7s %11s %5s %20s %20s %8s %24s %s\n",
		   "inode", "Permissions", "Links", "User ID", "Group ID", "Size",
		   "Last Modified", "Name");

	// read entries
	while ((dir = readdir(dp)) != NULL)
	{
		// ignore empty records
		if (dir->d_ino != 0)
		{
			char *filename;			   // store filename
			struct stat filestat;	   // store file stats
			filename = dir->d_name;	   // get filename
			stat(filename, &filestat); // get file stats

			// checks through all filters, if any filter is not passed, file is not printed
			if (
				((older != 0 && fileIsOlder(older, filestat.st_mtime) == 1) || older == 0) &&
				((younger != 0 && fileIsYounger(younger, filestat.st_mtime) == 1) || younger == 0) &&
				((larger != 0 && filestat.st_size > larger) || larger == 0) &&
				((smaller != 0 && filestat.st_size < smaller) || smaller == 0))
			{
				printFileStats(filename, filestat); // print file stats
			}
		}
	}
	closedir(dp); // close directory
	return 0;	  // exit
}
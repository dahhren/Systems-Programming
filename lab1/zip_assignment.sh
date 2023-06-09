#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#
# Zip Assignment Directory with username
#
# Name:zip_assignment.sh
#
# Written By: Darren Dsilva - January 2023
#
# Purpose: Compresses and creates a zip file of your assignment directory and saves in the home directory. It also names the file depending on the user's username.
#
# Usage: zip-assignment.sh <assignment-directory>
#
# Parameters: $1 - Assignment directory you want to zip.
#
#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------

# Check if the first argument exists
if [ -z "$1" ]                              #Checks if argument entered by user is $1
then
echo "missing argument on the command line" #Prints missing argument is argument is not entered by user.
exit 1;
fi

zip -r $HOME/$USER.zip $1                   #Proceeds to Zip the file in the home directory with the filename that matches the user logged in.
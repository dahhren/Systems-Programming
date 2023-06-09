#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#
# Retrieving Data from server log files.
#
# Name:q2p1.sh
#
# Written By: Darren Dsilva - January 2023
#
# Purpose: Retrieves pages with the 404 error, and creates files with the top ten 404 error pages and top ten IP's.
#
# Usage: cat top_ten_404_pages.txt
#. . . . cat top_10_IPs.txt             ||| wget http://ipinfo.io/<desired-ip-address>                ||| cat <desired-ip-address> 
#. . . . Retrieves top 10 IPs           ||| fetches info from an IP address and creates a text file    ||| Retrieves information about the IP address.
#
# Parameters: $1 - Log file, or a whole directory with log files.
#
#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------

# This command navigates through the folders till it reaches lab1, and then finds out all the 404 errors, extracts and prints data in columns 6 and 7, which contain the 404 -
#- pages, sorts it and counts the number of times a line is repeated if it is, sorts again by number in descending order, and gives us the top ten 404 pages. It then outputs -
#- it to a textfile called top_ten_404_pages.txt.
cat /home/COIS/3380/lab1/* | grep 404 | awk '{print $6, $7}' | sort | uniq -c | sort -nr | head -n 10 > top_ten_404_pages.txt

# This command navigates through the folders till it reaches lab1, and then finds out all the 404 errors, extracts and prints data in column 1, which contain the top 10 -
#- 404 pages, sorts it and counts the number of times a line is repeated if it is, sorts again by number in descending order, and gives us the top ten IP's pages. It then outputs -
#- it to a textfile called top_10_IPs.txt.
cat /home/COIS/3380/lab1/* | grep 404 | awk '{print $1}' | sort | uniq -c | sort -nr | head -n 10 > top_10_IPs.txt

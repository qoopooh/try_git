#!/bin/sh

clear
echo "Hello $USER"
echo "Today is $(date)"
echo "Number of user login: " ; who | wc -l # Do not work on wrt
echo "Calendar"
cal # Do not work on wrt

myname=Vivek
myos=TroubleOS
myno=5
echo "My name is $myname"
echo "My os is $myos"
echo "My number is $myno, can you see this number"

exit 0


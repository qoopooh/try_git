#!/bin/sh

clear
echo "Hello $USER, you're in `pwd`"
echo "Today is $(date)"
echo -n "Number of user login: " ; who | wc -l # output on same line | do not work on wrt
echo -e "This os is \c"   # \c for suppressing new line
uname -a
echo "Calendar"
cal # Do not work on wrt

echo -n 'Your first name please: '
read name

myos=TroubleOS
myno=5
echo "My name is $name"
echo "My os is $myos"
echo "My number is $myno, can you see this number"

exit 0


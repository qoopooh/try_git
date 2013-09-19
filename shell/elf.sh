#!/bin/sh

if [ $# -ne 1 ]
then
echo "$0 : You must give one integer"
exit 1
fi

if [ $1 -gt 0 ]; then
  for (( i=0; i<=$1; ++i )); do
    echo "Welcome $i times"
  done
  for i in 1 2 3;do
    echo "Thank you $i times"
  done
elif [ $1 -lt 0 ]; then
  echo "$1 number is negative"
elif [ $1 -eq 0 ];then
  echo "$1 number is zero"
else
  echo "$1 is not number"
fi

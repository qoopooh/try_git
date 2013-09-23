#!/bin/sh

PressNext () {
  echo "Press a key . . . \c"; read key
}

while :
do
  clear
  echo '------------------------------'
  echo ' Main Menu '
  echo '------------------------------'
  echo '[1] Show Todays date/time'
  echo '[2] Show files in current directory'
  echo '[3] Show calendar'
  echo '[4] Start editor to write letters'
  echo '[5] Exit/Stop'
  echo '======================='
  echo "Enter your menu choice [1-5]: \c" # don't go to new line (for mac)
  read yourch
  case $yourch in
    1) echo "Today is `date`"; PressNext;;
    2) echo "Files in `pwd`"; ls -l; PressNext;;
    3) cal ; PressNext;;
    4) vi ;;
    5) exit 0;;
    *) echo 'Please select choice 1, 2, 3, 4 or 5';
      PressNext;;
  esac
done


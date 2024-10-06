#!/bin/bash

#set(ORACLE ON) -> set(ORACLE OFF)

FILE_PATH="CMakeLists.txt"

NOWSETTING=`cat $FILE_PATH | grep -n "set(ORACLE " | awk -F ":" '{print $2}' | awk '{print $2}' | awk -F ")" '{print $1}'`
if [ $NOWSETTING == "ON" ]; then
    sed -i 's/set(ORACLE ON/set(ORACLE OFF/g' $FILE_PATH
    echo "ORACLE OFF"
else
    sed -i 's/set(ORACLE OFF/set(ORACLE ON/g' $FILE_PATH
    echo "ORACLE ON"
fi
#!/bin/bash

if [ $# != "2" ]
then
	echo "Usage: $0 FILE ADDR"
	exit 1
fi

if [ ! -f $1 ]
then
	echo "No such file or directory: $1"
	exit 1
fi

ADDR=$2
ABS_PATH="$PWD/$1"
echo "Flashing $ABS_PATH"
OCD_PATH=$(which openocd)
if [ ! -x "$OCD_PATH" ]
then
	echo "openocd not in PATH"
	exit 1
fi

#openocd &

sleep 2

(
echo "reset halt"
sleep 1
echo "flash probe 0"
sleep 1
echo "flash write_image erase $ABS_PATH $ADDR"
sleep 1
echo "reset"
) | telnet localhost 4444 2>/dev/null

exit 0

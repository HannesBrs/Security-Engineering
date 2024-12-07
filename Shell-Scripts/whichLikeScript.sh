#!/bin/sh

#whichLikeScript

#this scipt emulates the which command
#and checks whether a programm is executable and
#saved somewhere in the paths from PATH

#usage ./whichLikeScript.sh [file]
#example ./whichLikeScript.sh ls



if [ "$#" -ne 1 ]
	then
		echo "Error: Illegal number of files provided; usage: ./whichLikeScript.sh [program]"
		exit 1
fi



prog=$1
found=0



for pathParam in $(echo "$PATH" | sed 's/:/ /g')
do
	if [ -x "$pathParam/$prog" ]
		then
			echo "Program '$prog' found executable in '$pathParam'"
			found=1
	fi
done



if [ "$found" -eq 0 ]
	then
		echo "Programm '$prog' not found in PATH or not executable"
		exit 1
fi

exit 0


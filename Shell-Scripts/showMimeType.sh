#!/bin/sh

#showMimeTpye

#this script prints the file mime-type for all
#provided arguemnts

#usage ./showMimeType [file1] [file2] [file3] ...

if [ "$#" -lt 1 ]
	then
		echo "Error: Illegal number of files provided; usage: ./showMimeType.sh [arg1] [arg2 [arg3] ..."
		exit 1
fi

for filename in $@
	do
		if [ ! -f "$filename" ]
			then
				echo "Error: File '$filename' not found or unvalid"
				exit 1
		else
			file -b --mime-type $filename
		fi
	done

exit 0

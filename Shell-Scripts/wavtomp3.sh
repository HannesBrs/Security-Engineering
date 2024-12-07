#!/bin/sh

#wavtomp3

#this script converts a wav file to a mp3 file
#using ffmpeg

#usage ./wavtomp3.sh [file1] [file2] [file3] ...


if [ "$#" -lt 1 ]
	then
		echo "Error: Illegal number of files provided; usage: ./wavtomp3.sh [file1] [file2] [file3] ..."
		exit 1
fi

for filename in "$@"
do

	if [ ! -f "$filename" ]
		then
			echo "Error: File '$filename' not found or unvalid"
		elif ! file -b --mime-type "$filename" | grep -q "audio/x-wav"
			then
				echo "Error: File '$filename' must be a wav file"
		else
			ffmpeg -i "$filename" "${filename%.wav}.mp3"
	fi

done


exit 0


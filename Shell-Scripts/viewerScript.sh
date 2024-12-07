#!/bin/sh

#viewerScript

#this script opens a suitable programm in order to
#view a certain
#type of file

#possible types for this viewer script:
#-image (xv)
#-pfd (xpdf)
#-text (less)
#-open document (libreoffice)

#usage ./viewerScript.sh [file]

filename=$1

#file $filename

if [ "$#" -ne 1 ]
	then
		echo "Error: Illegal number of files provided; usage: ./viewerScript.sh [file]"
		exit 1
fi


if [ ! -f "$filename" ]
	then
		echo "Error: File not found or unvalid"
		exit 1
fi


filetype=$(file -b --mime-type "$filename")

case "$filetype" in
	image/jpeg)
		echo "filetype is image"
		xviewer "$filename"
		;;
	application/pdf)
		echo "filetype is pdf"
		xpdf "$filename"
		;;
	text/plain)
		echo "filetype is text"
		less "$filename"
		;;
	application/vnd.oasis.opendocument.text)
		echo "filetype is Opendocument Text"
		libreoffice "$filename"
		;;
	*)
		echo "Error: filetype not provided by this script (only image, pdf, text, OpenDocument)"
		;;
esac



exit 0

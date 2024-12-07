#!/bin/bash

#Unterbrechen des Programms mit Strg+C verhindern
trap '' SIGINT

tandir="TAN"

while true; do

	#Username und Tan einlesen
	echo -n "Username: "
    	read username
    	echo -n "TAN: "
    	read tan

	#Den eingegebenen Tan hashen
	tanHashed=$(echo -n "$tan" | sha256sum | awk '{print $1}')

	#Dem Username entsprechende TAN Datei und deren Laenge bestimmen
    	tanfile="$tandir/$username.txt"
	fileLength=$(cat "$tanfile" | wc -l)

	#Testen, ob der User existiert (es muss seine Datei vorhanden sein)
	if [ ! -f "$tanfile" ]; then
        	echo "Zugang verweigert: User existiert nicht."
        	continue
    	fi

	#Testen, ob noch genug TANs vorhanden sind
	if [ "$fileLength" -lt 2 ]; then
		echo "Keine TANs mehr uebrig."
		continue
	fi

	#Der <in der Bank gespeicherte Hashwert> (letzter der Datei)
	currTan=$(tail -n 1 "$tanfile")

	#Den Hashwert mit dem Hashwert des eingebenen TANs vergleichen
	#Der vorletzte Tan aus der Datei ergibt gehasht den letzten aus der Datei
	if [ "$tanHashed" = "$currTan" ]; then
		echo "Zugang gestattet."

		#Letzte Zeile also den <in der Bank gespeicherten Hashwert> loeschen
		#Somit ist der vom Nutzer gueltig eingegebene Tan der neue <in der Datei gespeicherte Hashwert>
		sed -i '$ d' "$tanfile"
		continue

	else
		echo "Zugriff verweigert: ungueltiger TAN."
		continue
	fi

done

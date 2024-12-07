#!/bin/sh

#Anzahl an Kommandozeilenparameter pruefen
if [ "$#" -ne 2 ]; then
	echo "Benutzung: $0 [Username] [Anzahl TANs n]"
	exit 1
fi

username="$1"
n="$2"
tandir="TAN"

#TAN Verzeichnis erstellen, falls nicht schon vorhanden
mkdir -p "$tandir"

tanfile="$tandir/$username.txt"

#tanfile loeschen um TANs zurueckzusetzen
rm -f "$tanfile"

#random Startwert generieren lassen
randStart=$(openssl rand -hex 16)

#aktueller TAN
current="$randStart"

#TANs erstellen (Einbahnstrasse)
for i in $(seq 1 "$n"); do

	next=$(echo -n "$current" | sha256sum | awk '{print $1}')
	echo "$next" >> "$tanfile"
	current="$next"
done

echo "$n TANs fuer User $username generiert"


#!/bin/bash

echo "Dostępne dyski:"
lsblk

read -p "Wprowadź nazwę partycji: " PARTYCJA

MIEJSCE=$(df -h | grep "$PARTYCJA" | awk '{print $4 " wolne z " $2}')

if [ -z "$MIEJSCE" ]; then
  echo "Podana partycja nie istnieje lub nie ma do niej dostępu."
  exit 1
fi

DATA=$(date +"%Y%m%d-%H%M%S")
PLIK="Report_${DATA}.txt"

echo "Dostępne dyski:" > "$PLIK"
lsblk >> "$PLIK"
echo -e "\nMiejsce dostępne na partycji $PARTYCJA: $MIEJSCE" >> "$PLIK"

echo "Plik o nazwie $PLIK został zapisany w $(pwd)."


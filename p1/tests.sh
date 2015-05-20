#!/bin/bash

make

INPUT=$1
IFS=,
[ ! -f $INPUT ] && { echo "$INPUT file not found"; exit 99; }

while read host port data name
do
	echo $data | ./p1 $host $port > ${name}.out
	
done < $INPUT
IFS=$OLDIFS




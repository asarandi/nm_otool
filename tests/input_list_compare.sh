#!/bin/bash

#wildcard='binary-samples/Mac*'
#wildcard='testers/*'

if [ -z "$1" ]; then
	echo "usage: $0 <input_file>"
	exit
fi

inputfile='applications_dylib.list.txt'
outputf='DiffyDiff/'

prog1='../ft_nm '
ext1='.alex'
prog2='/usr/bin/nm '
ext2='.nemo'
hasher='/sbin/md5 -q '

sizecmd='stat -f %z '

red='\033[0;31m'
green='\033[0;32m'
yellow='\033[0;33m'
nc='\033[0m'

bad="$red hashes are not the same $nc"
good="$green hashes are the same $nc"

rm -rf $outputf
mkdir -p $outputf

while read f0; do
	f1=`echo $f0 | sed 's/.*\///'`

	o1=$outputf$f1$ext1
	$prog1 "$f0" > $o1
	o2=$outputf$f1$ext2
	$prog2 "$f0" > $o2

	hash1=`$hasher $o1`
	hash2=`$hasher $o2`

	if [ "$hash1" != "$hash2" ]; then
		echo -e "$bad : $f0"
		size1=`$sizecmd $o1`
		if [ "$size1" = "0" ]; then
			echo -e "$yellow deleting because empty $nc $o1 $o2"
			rm -f $o1
			rm -f $o2
		fi;
	else
		echo -e "$good : $f0"
		rm -f $o1
		rm -f $o2
	fi

done < $1

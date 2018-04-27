#!/bin/bash
# ./HGTFinder.sh [DB Blast] [Self Blast] [taxid] [out prefix] <q-value> <gff> <cluster hole size>
# [] -> required
# <> -> optional
#	for clustering

dir=$(dirname $0)"/"

if [ $# -ne 4 ] && [ $# -ne 7 ]
then
	echo "Usage"
	echo $dir"HGTFinder.sh [DB Blast] [Self Blast] [taxid] [out prefix] <q-value> <gff> <cluster hole size>"
	echo "Improper args"
	exit
fi

dbbl=""
sebl=""
tid=""
out=""
per=""
gff=""
chs=""

dbbl=$1
sebl=$2
tid=$3
out=$4
per="0.99"

if [ $# -eq 7 ]
then
	per=$5
	gff=$6
	chs=$7
fi

if [ $# -eq 4 ]
then
	$dir"HGTFinder" -d $dbbl -s $sebl -t $tid -o $out -r 0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9 > errOut
fi

if [ $# -eq 7 ]
then
	$dir"HGTFinder" -d $dbbl -s $sebl -t $tid -o $out -q $per -r 0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9 -g $gff -h $chs > errOut
fi
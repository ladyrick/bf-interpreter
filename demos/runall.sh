#!/usr/bin/env bash

BASEPATH=$(cd `dirname $0`/..; pwd)

g++ $BASEPATH/bfrun.cpp -o bfrun.bin -std=c++11 -O3

if [[ $? -ne 0 ]]; then
    echo build failed.
    exit
fi

for bf in  $(ls $BASEPATH/demos | grep .bf)
do
    echo demo $BASEPATH/demos/$bf
    $BASEPATH/bfrun.bin $BASEPATH/demos/$bf
    echo -------------------------------------
done

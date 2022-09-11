#!/bin/bash

OPTIONS=
TIMES=100
SUM=0
for i in {1..$TIMES};
do
    TEMPO=$(./elenco -v -$OPTIONS < input.txt | grep Tempo | cut -d" " -f4)
    echo $TEMPO
done
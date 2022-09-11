#!/bin/bash

OPTIONS=a
TIMES=1000.0
SUM=0.0
for i in $(seq 1 $TIMES);
do
    TEMPO=$(./elenco -v -$OPTIONS < input.txt | grep Tempo | cut -d" " -f4)
    SUM=$(bc <<< "$SUM + $TEMPO")
done

echo Tempo médio de execução
echo $(bc -l <<< "$SUM / $TIMES") ms
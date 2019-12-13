#!/bin/bash
for(( i=$1 ; $i<=$2 ; i=$i+1 )) do
  echo $i;
  ./main -g Graphs/$i-nodes.gml -s Graphs/svg/$i-nodes.svg -c $i
done
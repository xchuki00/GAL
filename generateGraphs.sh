#!/bin/bash
mkdir -p Graphs
for(( i=$1 ; $i<=$2 ; i=$i+1 )) do
  echo $i;
  ./tsp -g Graphs/$i-nodes.gml -s Graphs/svg/$i-nodes.svg -c $i
done
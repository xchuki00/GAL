#!/bin/bash
for i in {10..60}; do
  echo $i;
  ./main -g Graphs/$i-nodes.gml -s Graphs/svg/$i-nodes.svg -c $i
done
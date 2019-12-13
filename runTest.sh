#!/bin/bash
#./runTest.sh 5 20 out2.csv -> $1=from $2=to $3=outputfile
echo "Alg;N;Price;Time;Time1;Time2;Time2;" >$3
for ((i = $1; $i <= $2; i = $i + 1)); do
  out=$(./main -a 2 -i Graphs/$i-nodes.gml)
  a=($out)
  price=0
  time=0
  for index in "${!a[@]}"; do
    #    echo "$index ${a[index]}"
    #echo ${a[index]};
    if [ ${a[index]} == "PRICE:" ]; then
      price=${a[index + 1]}
    fi
    if [ ${a[index]} == "TIME:" ]; then
      time=${a[index + 1]}
      time=${time//"[ns]"/""}
    fi
  done
  echo "DT;$i;$price;$time;;;;" >>$3
done

for ((i = $1; $i <= $2; i = $i + 1)); do
  out=$(./main -a 1 -k 2 -i Graphs/$i-nodes.gml)
  a=($out)
  price=0
  time=0
  time1=0
  time2=0
  time3=0
  for index in "${!a[@]}"; do
    if [ ${a[index]} == "PRICE:" ]; then
      price=${a[index + 1]}
    fi
    if [ ${a[index]} == "TIME:" ]; then
      time=${a[index + 1]}
      time=${time//"[ns]"/""}
    fi
    if [ ${a[index]} == "TIME1:" ]; then
      time1=${a[index + 1]}
      time1=${time1//"[ns]"/""}
    fi
    if [ ${a[index]} == "TIME2:" ]; then
      time2=${a[index + 1]}
      time2=${time2//"[ns]"/""}
    fi
    if [ ${a[index]} == "TIME3:" ]; then
      time3=${a[index + 1]}
      time3=${time3//"[ns]"/""}
    fi
  done
  echo "k-OPT;$i;$price;$time;$time1;$time2;$time3;" >>$3
done
for ((i = $1; $i <= $2; i = $i + 1)); do
  if (( $i < 12 )); then
    out=$(./main -a 3 -i Graphs/$i-nodes.gml)
    a=($out)
    price=0
    time=0
    for index in "${!a[@]}"; do
      if [ ${a[index]} == "PRICE:" ]; then
        price=${a[index + 1]}
      fi
      if [ ${a[index]} == "TIME:" ]; then
        time=${a[index + 1]}
        time=${time//"[ns]"/""}
      fi
    done
    echo "BF;$i;$price;$time;;;;" >>$3
  fi
done

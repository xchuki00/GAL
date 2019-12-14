#!/bin/bash
#./runTest.sh out2.csv
echo "Alg;N;Price;Time;K;Time1;Time2;Time2;" >$1
#for ((i = $1; $i <= $2; i = $i + 1)); do
for file in Graphs/*.gml; do
  out=$(./main -a 2 -i $file)
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
  file=${file//"-nodes.gml"/""}
  file=${file//"Graphs/"/""}
  echo "DT;$file;$price;$time;;;;;" >>$1
done
for ((k = 2; $k <= 5; k = $k + 1)); do
  #  for ((i = $1; $i <= $2; i = $i + 1)); do
  for file in Graphs/*.gml; do
    out=$(./main -a 1 -k $k -i $file)
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
    file=${file//"-nodes.gml"/""}
    file=${file//"Graphs/"/""}
    echo "k-OPT;$file;$price;$time;$k;$time1;$time2;$time3;" >>$1
  done
done
#for ((i = $1; $i <= $2; i = $i + 1)); do
for file in Graphs/*.gml; do
  n=${file//"-nodes.gml"/""}
  n=${n//"Graphs/"/""}
  if (($n <= 14)); then
    out=$(./main -a 3 -i $file)
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
    echo "BF;$n;$price;$time;;;;;" >>$1
  fi
done

#!/bin/bash
ogdfUrl="https://ogdf.uos.de/wp-content/uploads/2019/04/ogdf-snapshot-2018-03-28.zip";
buildDir="build";
ogdfDir="ogdf";
mkdir -p $ogdfDir
cd $ogdfDir;
FILE=./ogdf-snapshot-2018-03-28.zip
if [ ! -f "$FILE" ]; then
    wget $ogdfUrl;
fi
unzip -u ogdf-snapshot-2018-03-28.zip
cp -r OGDF-snapshot/* ./
rm -rf OGDF-snapshot

cd ..
mkdir -p $buildDir;
cd $buildDir;
cmake ../$ogdfDir;
make all

cd ..;

cmake .;
make
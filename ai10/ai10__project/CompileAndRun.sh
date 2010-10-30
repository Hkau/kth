#!/bin/bash

cd code
cd MakeDat
make
mv Sokoban++ ../..
cd ..
./Clean.sh
cd ..
./Sokoban++


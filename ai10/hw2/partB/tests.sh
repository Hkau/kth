#!/bin/bash
for i in 10 100 500 1000 2000 3000 4000 5000 6000 7000 8000 9000 10000
do
	gcc -O3 -o amazon amazon.c -DNUM_AMAZON=$i 2> /dev/null
	echo "Running for Board of size $ix$i"
	time ./amazon
done
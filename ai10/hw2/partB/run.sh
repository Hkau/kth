#!/bin/sh

#gcc -g -o amazon amazon.c -DNUM_AMAZON=$1
gcc -O3 -o amazon amazon.c -DNUM_AMAZON=$1

./amazon

#!/bin/sh

gcc -g -o amazon amazon.c -DNUM_AMAZON=$1
gcc -O2 -o amazon amazon.c -DNUM_AMAZON=$1

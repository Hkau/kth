#!/bin/sh

mpirun -n $((2*$1+1)) ./stable_marriage

#!/usr/bin/ruby

require 'qsort.rb'
v = [4711, 25, 25, 23, 17, 121, 99, 72, 431, 42]
array = (1..1000000).map { rand() }
qsort array


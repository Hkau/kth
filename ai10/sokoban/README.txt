./sokoban [n]

	Runs level n, if supplied, else asks for board number on execution.

compileAndRun.sh is used to compile and run the code. This creates a `sokoban' executable (and runs it). lookup3.c is c code for generating hashes, this is compiled separately with gcc. The rest is compiled and linked with g++. -O3 is an important flag, for performance.

stats.txt provides solutions and statistics for our final runs. All boards were finished within 60 seconds, and the total test (136 levels) took about 4:30.

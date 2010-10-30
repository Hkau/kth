There are 17 files in 5 directories. Six .cpp source code and 6 .h header sources file in Directory Board, one .h header source file in directory Heuristics, one .cpp source code and one .h source file in directory Search.

=================================================================================================================

code/src/Board/Board.cpp			Class Board - implementations of methods
code/src/Board/Board.h				Class Board - definition of the class
code/src/Board/BoardLocal.cpp			Class BoardLocal - implementations of methods
code/src/Board/BoardLocal.h			Class BoardLocal - definition of the class
code/src/Board/MABoardGraph.cpp			Class MABoardGraph - implementations of methods
code/src/Board/MABoardGraph.h			Class MABoardGraph - definition of the class
code/src/Board/MABoardGuru.cpp			Class MABoardGuru - implementations of methods
code/src/Board/MABoardGuru.h			Class MABoardGuru - definition of the class
code/src/Board/MABoardSearch.cpp		Class MABoardSearch - implementations of methods
code/src/Board/MABoardSearch.h			Class MABoardSearch - definition of the class
code/src/Board/StaticBoard.cpp			Class StaticBoard - implementations of methods
code/src/Board/StaticBoard.h			Class StaticBoard - definition of the class
code/src/Heuristics/SimpleChessGuru.h		Class SimpleChessGuru - definition and implementation of the class
code/src/Search/AStar.cpp			Class AStar - implementations of methods
code/src/Search/AStar.h				Class AStar - definition of the class
code/src/main.cpp				Main program
code/src/util.h					C++ header of some handy utils
=================================================================================================================

To compile the code, run the provided compileAndRun.sh file in linux OS as bellow:

$ bash CompileAndRun.sh

To run the program against board number N just type Sokoban++ N in the code directore.

Note that the search is restricted to one minute (60 sec) because of server restriction. If you want the search not to
give up please increase the allowed time in code/src/Board/MABoardGraph.h line 16

Please feel free to contact me should you got any error or have any question.

Thank You,
Soheil Damangi <damangir@kth.se>

% :- use_module(library(wgraphs)).
% main(fil, från, till, Väg, Avstånd).

main(F, Start, Dest, ShortPath, ShortDist):-
	open(F, read, ID),
	read_edges(G, ID),
	close(ID),
	% manipulate Graph
	findall([Path, Dist], find_path(Path, Dist, _, G, Start, Dest), Paths),
	shortest_path([ShortPathCode, ShortDist],Paths),
	translate_code_array(ShortPath, ShortPathCode).

translate_code_array([],[]).

translate_code_array([X|XT], [C|T]):-
	translate_code_array(XT, T),
	codearray_string(X, C).

codearray_string([], []).

codearray_string([X|XT], [C|T]):-
	codearray_string(XT, T),
	char_code(X, C).

shortest_path(P,[P]).

shortest_path([P, Dist], [[P, Dist]|Tail]):-
	shortest_path([_,PDist], Tail),
	Dist < PDist, !.

shortest_path(Path, [_|Tail]):-
	shortest_path(Path, Tail).

read_edges(G, ID):-
	read_edge(E, ID),
	read_prepend_edges(G, E, ID).

read_prepend_edges([E], E, ID):-
	get_code(ID, C),
	put_back_non_eof(ID, C),
	C == -1,
	!.

read_prepend_edges(G, E, ID):-
	read_edge(E2, ID),
	read_prepend_edges(G1, E2, ID),
	G = [E | G1].

put_back_non_eof(_, -1):- !.

put_back_non_eof(ID, C):-
	unget_code(ID, C).

read_edge([P1, P2, Dist, Name], ID):-
	read_word(P1, ID),
	read_word(P2, ID),
	read_word(D, ID),
	to_int(Dist, D),
	read_word(Name, ID).

to_int(I, Str):-
	to_int_internal(I, Str, 0).

to_int_internal(_, [X|_], _):-
	X < 48,
	!,
	fail.

to_int_internal(_, [X|_], _):-
	X > 57,
	!,
	fail.

to_int_internal(I, [X|T], Irest):-
	%to_int_internal(I, T, (Irest*10+X-48)). % går inte denna? >: what?
	Foo is Irest*10+X-48,
	to_int_internal(I, T, Foo).

to_int_internal(I, [], I).

read_word(W, ID):-
	get_code(ID, C),
	read_prepend_word(W, C, ID).%, write(W),nl.

read_prepend_word([], C, _):-
	member(C, [-1, 9, 10]), !.

read_prepend_word(W, C, ID):-
	get_code(ID, Ch),
	read_prepend_word(W1, Ch, ID),
	W = [C | W1].

find_path([], 0, _, _, Start, Start):- !.

find_path(Path, Dist, Old, [[EStart, Eend, EDist, EPath]|GTail], Start, Dest):-
	%write(EStart), write(Eend), nl,
	go_from([EStart, Eend, EDist, EPath], Start, Next),
	find_path(Path2, Dist2, Old, GTail, Next, Dest),
	append_non_redundant(EPath, Path2, Path),
	Dist is Dist2+EDist.

find_path(Path, Dist, [E|Old], [E|ETail], Start, Dest):-
	find_path(Path, Dist, Old, ETail, Start, Dest).

append_non_redundant(EPath, [EPath|Path2], [EPath|Path2]):- !.

append_non_redundant(EPath, Path2, Path):-
	append([EPath], Path2, Path).

go_from([Start, Next|_], Start, Next).
go_from([Next, Start|_], Start, Next).

draw_graph(F):-
	open(F, read, ID),
	read_edges(G, ID),
	close(ID),
	write(G).

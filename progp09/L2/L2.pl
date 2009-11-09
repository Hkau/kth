write_crossword([]):-
	format("~n", []).

write_crossword([R|T]):-
	format("~n", []),
	format(R,[]),
	write_crossword(T).
	

crossword(X,C):-
	Cempty = ["ooooo", "o o o", "ooooo", "o o o", "ooooo"],
	insert_row_1(X, X1, Cempty, C1),
	insert_col_1(X1, X2, C1, C2),
	insert_row_2(X2, X3, C2, C3),
	insert_col_2(X3, X4, C3, C4),
	insert_row_3(X4, X5, C4, C5),
	insert_col_3(X5, _, C5, C).

	% if any other words match row 2, intersect with 3, 1
	% insert any of those
	% if any other words match column 2, that is don't intersect with 1,3; 3, 3
	% insery any of those
	% if any other words match row 3, intersect 5,1; 5,3
	% insert any of those
	% if any other words match column 5, !intersect 1,5; 3,5; 5,5
	% insert any of those

insert_row_1([X|T], T, [_|CTail], [Cnew|CTail]):-
	Cnew = X.

insert_row_1([X|T], Xleft, C, C2):-
	insert_row_1(T, X2, C, C2),
	Xleft = [X|X2].

%optimering, sorterad ordlista..
%insert_col_1([[X|_]|_], _, [[C|_]|_], _):-
%	X > C,
%	fail.

insert_col_1([[X1, X2, X3, X4, X5]|T], T, [[C|Tail],[_|Tail2], [_|Tail3], [_|Tail4], [_|Tail5]], [[C|Tail],[X2|Tail2],[X3|Tail3],[X4|Tail4],[X5|Tail5]]):-
	X1 = C.

insert_col_1([X|T], Xleft, C, C2):-
	insert_col_1(T, X2, C, C2),
	Xleft = [X|X2].

insert_row_2([[X1|XT]|T], T, [C1, C2, [C3|_] | CT], [C1, C2, [X1|XT]| CT]):-
	X1 = C3.

insert_row_2([X|T], Xleft, C, C2):-
	insert_row_2(T, X2, C, C2),
	Xleft = [X|X2].

insert_col_2([[X1, X2, X3, X4, X5]|T],T,[[C11, C12, C13, C14, C15], [C21, C22, _, C24, C25], [C31, C32, C33, C34, C35], [C41, C42, _, C44, C45], [C51, C52, _, C54, C55]], [[C11, C12, C13, C14, C15], [C21, C22, X2, C24, C25], [C31, C32, C33, C34, C35], [C41, C42, X4, C44, C45], [C51, C52, X5, C54, C55]]):-
	X1 = C13,
	X3 = C33.

insert_col_2([X|T], Xleft, C, C2):-
	insert_col_2(T, X2, C, C2),
	Xleft = [X|X2].

insert_row_3([[X1,X2,X3|XT]|T], T, [C1, C2, C3, C4, [C51, _, C53, _, _]], [C1, C2, C3, C4, [X1, X2, X3|XT]]):-
	X1 = C51,
	X3 = C53.

insert_row_3([X|T], Xleft, C, C2):-
	insert_row_3(T, X2, C, C2),
	Xleft = [X|X2].

insert_col_3([X|T], Xleft, C, C2):-
	insert_col_3(T, X2, C, C2),
	Xleft = [X|X2].

insert_col_3([[X1, X2, X3, X4, X5]|T],T,[[C11, C12, C13, C14, C15], [C21, C22, C23, C24, _], [C31, C32, C33, C34, C35], [C41, C42, C43, C44, _], [C51, C52, C53, C54, C55]], [[C11, C12, C13, C14, C15], [C21, C22, C23, C24, X2], [C31, C32, C33, C34, C35], [C41, C42, C43, C44, X4], [C51, C52, C53, C54, C55]]):-
	X1 = C15,
	X3 = C35,
	X5 = C55.


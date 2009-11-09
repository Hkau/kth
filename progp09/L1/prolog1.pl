count_leaves(l(_), N) :-
	N is 1.

count_leaves(b(A, B), N):-
	count_leaves(A, N1),
	count_leaves(B, N2),
	N is N1 + N2.

leaves(l(X), L):-
	L = [X].

leaves(b(A, B), L):-
	leaves(A, L1),
	leaves(B, L2),
	append(L1, L2, L).

%build_tree([L], T):-
%	T = l(L).

%build_tree([L|R], T):-
%	build_tree(R, T1),
%	T = b(l(L), T1). 

build_tree(L, T):-
	leaves(T, L).

max(X,Y,Z):-
	X > Y,
	Z is X.

max(X,Y,Z):-
	X =< Y,
	Z is Y.

height(l(_), H):-
	H is 1.

height(b(A,B), H):-
	height(A, H1),
	height(B, H2),
	max(H1, H2, MaxH),
	H is MaxH + 1.

balanced(l(_)).

balanced(b(A, B)):- 
	balanced(A),
	balanced(B),
	height(A, H1),
	height(B, H2),
	abs(H1-H2) =< 1.

history(
b(
  l(yeast),
  b(
    b(
      l(fly),
      l(mosquito)
     ),
    b(
      l(frog),
      b(
	l(chicken),
	b(
	  l(mouse),
	  b(
	    l(human),
	    l(chimp)
	   )
	 )
       )
     )
   )
 )).

tree1(l(enkelt)).
tree2(b(l(1),l(2))).
tree3(b(l(1),b(l(2),l(3)))).
tree4(b(b(l(1),l(2)), b(l(3), l(4)))).
tree5(b(l(1), b(l(2), b(l(3),b(l(4),l(5)))))).


writetree(T) :- writetree(T, 0).
writetree(l(X), N) :-
	indent(N),
	write(X).
writetree(b(X,Y), N) :-
	indent(N),
	write('('), nl,
	Next is N + 2,
	writetree(X, Next),
	write(', '), nl,
	writetree(Y, Next), nl,
	indent(N),
	write(')').

indent(0).
indent(N) :- N>0, write(' '), Next is N-1, indent(Next).

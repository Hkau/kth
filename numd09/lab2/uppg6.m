%asdf these are soem hax0rz comments

% y'' - t*y' + e.^(-t/2)*y = t*cos(t)

hold on
for iter = 0:3

	N = 2^iter*10+1;

	h = 4/N;

	n = N-1

	t = h*(1:n)';

	T = [0; t; 4];

	dia = h^2*exp(-t/2)-2;
	sup = 1-t(1:n-1)*h/2;
	sub = 1+t(2:n)*h/2;
	b = (h^2*t.*cos(t));
	% b1 ok
	b(n) = b(n)-(1-t(end)*h/2)*0.8;

%	dia
%	sup
%	sub
%	b

	y = tridia(dia, sup, sub, b);

	Y = [0; y; 0.8];
	plot(T,Y)

end

axis equal

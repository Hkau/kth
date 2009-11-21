hold on
z = [8 9 0 2 2 4 0 8 1 4] + 1;
z = [z z(1)];

v = linspace(0, 2*pi, length(z));

title('Labb 1.3a,b')

stem(v, z)

powers = 0:length(v)-1;
k = ((repmat(v', 1, length(v)).^repmat(powers, length(powers), 1)) \ z')';;

x = linspace(0, 2*pi, 512);

y = [];

for i = 1:length(x)
	y = [y sum(k.*repmat(x(i), 1, length(powers)).^powers)];
end

%plot(x, y)

%del b

k = [0];

h = v(2);

for i = 2:length(z)-1
	k = [k (z(i+1)-z(i-1))/(2*h)]; %y-värdet för före och efter, delat med avståndet mellan varje punkt (h)
end

k = [k 0];

y = [];

for i = 1:length(z)-1
	y = []
	dy = z(i+1) - z(i);
	for t = linspace(0,1,16)
		g = h*k(i)-dy;
		c = 2*dy-h*(k(i)+k(i+1));
		y = [y (z(i)+t*dy+t*(1-t)*g+t*t*(1-t)*c)];
	end
	plot((i-1)*2*pi/10+linspace(0, 2*pi/10, length(y)), y)
end

niceangle = linspace(0, 2*pi, length(y));
%plot(niceangle, y, 'k');

% del c

figure(2)
title('Labb 1.3c')
hold on

angle = linspace(0,2*pi,128);

plot3(cos(angle), sin(angle), zeros(size(angle)))

stem3(cos(v), sin(v), z, '*b')
stem3(cos(niceangle), sin(niceangle), y)

format long
a = 10;
bval = [7, 5, 3, 1];

printf("Omkretsintegraler:\n");

for b = bval
	omkr = @(v) sqrt(b.^2 +(a.^2 - b.^2)*sin(v).^2);
	b
	omkrets = quad(omkr, 0, 2*pi)
end

v = linspace(0, 2*pi, 4711);

x = (repmat(a, length(bval), 1)*cos(v))';
y = (bval'*sin(v))';

plot(x,y);

printf("\nDifferens (Integral, Ramanujan):\n");

c = @(a,b) (a-b).^2/(a+b).^2;
omkr_r = @(a,b) pi*(a+b)*(1+3*c(a,b)/(10+sqrt(4-3*c(a,b))));

for b = bval
	omkr = @(v) sqrt(b.^2 +(a.^2 - b.^2)*sin(v).^2);
	b
	diff = abs(quad(omkr, 0, 2*pi) - omkr_r(a, b))
end

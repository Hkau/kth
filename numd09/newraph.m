format long;
a = 1
da = 1;
% gissa inte jättefel så x blir negativt och kukar ur.
while(abs(da) > 0.00000001)
	f = 1.5*sin(a) - sin(a)^3 - 0.64;
	fprim = 1.5*cos(a) - 3*sin(a)^3*cos(a);
	da = -f / fprim;
	a = a + da
end

y = 0.64;
x = cos(a)^3;

plot(x,y,'*')


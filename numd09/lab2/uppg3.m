format long
% Gamla goda Ramanujan -400 då vi söker f(x) = 0 med sekantmetoden och vill ha f = 400
c = @(a,b) (a-b).^2/(a+b).^2;
ram = @(a,b) pi*(a+b)*(1+3*c(a,b)/(10+sqrt(4-3*c(a,b))));
f = @(a,b) ram(a,b) - 400;

diff = 1;

a = 160/2 % längd/2 = radien

b_old = 1;
b = 200;

while abs(f(a,b)) > 1e-20
	b
	b_new = b - (b-b_old)/(f(a,b)-f(a,b_old)) * f(a,b) %sekantmetoden beräknar nytt b
	b_old = b;
	b = b_new;
end
bredd = 2*b
ram(a,b)

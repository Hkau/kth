format long
fq=@(t) exp(cos(pi/2*t))./(2+sin(pi*t)+0.5*cos(pi/2*t));
trap = @(y) (y(1)+2*sum(y(2:end-1))+y(end))/2/(length(y)-1);
T=4;

x = linspace(0, 4, 42);

hold on
for i = 4*(-1:1)
       	plot(x+i, fq(x))
end
axis equal

n=20;
last=0;
cur=1;

while abs(last-cur)>=1e-14
	last=cur;
	n=2*n;

	x = linspace(0, T, n+1);
	f=fq(x);
	cur=trap(f)*T;
end

n
cur

quad(fq, 0, 4)


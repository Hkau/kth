format long

f = @(x) e.^(-x/3)./(2 - cos(pi*x));

range_end = 2.6
x = linspace(0, range_end, 42);

y = f(x);

plot(x, y)

angles = linspace(0, 2*pi, 30);

surf(
	repmat(x, length(angles), 1),
	repmat(y, length(angles), 1).*repmat(cos(angles)', 1, length(x)),
	repmat(y, length(angles), 1).*repmat(sin(angles)', 1, length(x)))
axis equal

circle_f = @(x) pi*f(x).^2;
trap = @(y) (y(1)+2*sum(y(2:end-1))+y(end))/2/(length(y)-1);

x = 0:0.1:range_end;
V1 = trap(circle_f(x))*range_end
x = 0:0.05:range_end;
V2 = trap(circle_f(x))*range_end
S = V2 + (V2-V1)/3

Vquad = quad(circle_f, 0, range_end)

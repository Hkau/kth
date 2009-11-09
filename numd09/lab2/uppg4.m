% hattar hatten. :>

R = 23
L = 23

xp = @(r, v1, v2) (R-r)*cos(v1)+r*cos(v1+v2);
yp = @(r, v1, v2) (R-r)*sin(v1)+r*sin(v1+v2);

x = [8; pi/4; pi/4]; % r, v1, v2
maxdiff = 1;

xend = 11.5
yend = 18
f1 = @(r, v1, v2) R*v1+r*v2-L;
f2 = @(r, v1, v2) (R-r)*cos(v1)+r*cos(v1+v2)-xend;
f3 = @(r, v1, v2) (R-r)*sin(v1)+r*sin(v1+v2)-yend;

iter = 0;

while maxdiff > 1e-10 && iter < 10
	iter
	F = [
		f1(x(1), x(2), x(3))
		f2(x(1), x(2), x(3))
		f3(x(1), x(2), x(3))];

	J = [ % Jacubian maddafakka
		x(3), R, x(1)
		cos(x(2)+x(3))-cos(x(2)), -sin(x(2))*R-x(1)*sin(x(3)+x(2))+x(1)*sin(x(2)), -x(1)*sin(x(2)+x(3))
		sin(x(2)+x(3))-sin(x(2)), cos(x(2))*R+x(1)*cos(x(3)+x(2))-x(1)*cos(x(2)), x(1)*cos(x(2)+x(3))]; 
		%cos(x(2)+x(3))-cos(x(2)), (x(1)-R)*sin(x(2))-x(1)*sin(x(2)+x(3)), -x(1)*sin(x(2)+x(3))
		%sin(x(2)+x(3))-sin(x(2)), (R-x(1))*cos(x(2))-x(1)*x(3)*cos(x(2)+x(3)), x(1)*cos(x(2)+x(3))];

	dx = -J\F;

	x = x+dx

	maxdiff = max(abs(dx))

	iter = iter+1;
end

v1 = linspace(0, x(2), 4711);
v2 = linspace(0, x(3), 4711);

r = x(1);

% Plotta, inkl kända start- och slutvärden.
hold on

plot(xp(r, v1, v2), yp(r, v1, v2))

plot(23, 0, '*')
plot(11.5, 18, '*')

axis equal

clear
clf

tol = odeset('RelTol',1e-6);

%% Duffings ekvation %%
% x'' = a*cos(t) + x - x^3 -b*x', x(0) = 0, x'(0) = 1
a = 7.5;
b = 0.15;
f = @(t,u) [u(2); a*cos(t)+u(1)-u(1)^3-b*u(2)];

%% ODE45 lösning %%
ystart = [0 1];
span = [0 24];
[t y] = ode45(f, span, ystart, tol);
subplot(2,2,1);plot(t,y(:,1),'r');hold on;
subplot(2,2,2);plot(t,y(:,2),'r');

%% Euler %%
tslut=24;
t=0;
y=[0 1]; T=t; Y=y;
h=0.002;n=tslut/h;
for i=1:n
    fnc = f(t,y); y=y+h*fnc'; t=t+h; T=[T; t]; Y=[Y; y];
end
subplot(2,2,3);plot(T, Y(:,1),'b');

tslut=24;
t=0;
y=[0 1]; T=t; Y=y;
h=0.001;n=tslut/h;
for i=1:n
    fnc = f(t,y); y=y+h*fnc'; t=t+h; T=[T; t]; Y=[Y; y];
end
subplot(2,2,4);plot(T, Y(:,1),'g');
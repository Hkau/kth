clear
clf

tol = odeset('RelTol',1e-6);

%% a 1 %%
f = @(x,u) [u(2); (u(1)/9 - pi*u(1)*exp(x/3)*(2*u(2)*sin(pi*x) + pi*u(1)*cos(pi*x)))];

ystart = [1 -1/3];
span = [0 2.6];
[t,y] = ode45(f, span, ystart, tol);
subplot(2,2,1);plot(t,y(:,1),'b'); hold on;
subplot(2,2,3);plot(t,y(:,2),'r');
ys1=y(end,1)

%% a 2 %%
f = @(x,u) [u(2); (u(1)/9 - pi*u(1)*exp(x/3)*(2*u(2)*sin(pi*x) + pi*u(1)*cos(pi*x))); pi*u(1)^2];

ystart = [1 -1/3 0];
span = [0 2.6];
[t,y] = ode45(f, span, ystart, tol);
subplot(2,2,2);plot(t,y(:,3),'g');
ys2=y(end,3)


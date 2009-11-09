clear; clf;
format long;

%% Rita upp de givna punkterna %%
x = [1.1 1.5 2.2 3.2 4.5 4.8]';
y = [0.7 1.9 3.1 3.0 2.3 1.0]';

hold on; axis equal, axis([0 6 -2 4]);
plot (x,y,'rp');


%% Approximera en anpassning av cirkeln %%

% Cirkel = (x-xc)^2 + (y-yc)^2 - R^2 = 0

% Godtyckliga gissningar
xc=3; yc=2.5; R=2; 

% Okända variabler
p=[xc yc R]';

diffOld = 2;
diffNow = 3;

diff = 42;
% Gauss-Newton enligt boken
while diff > 0
    xd=x-xc; yd=y-yc;
    f=xd.^2/R^2+yd.^2/R^2-1;
    
    % Diff-beräkning
    diffOld = diffNow;
    diffNow = norm(f);
    diff = abs(diffNow-diffOld)
    
    J=-2*[xd/R^2 yd/R^2 xd.^2/R^3];
    dp=-J\f;
    p=p+dp;
    xc=p(1); yc=p(2); R=p(3);
end


%%  %%
v=0:2*pi/60:2*pi;
plot(xc+R*cos(v),yc+R*sin(v));

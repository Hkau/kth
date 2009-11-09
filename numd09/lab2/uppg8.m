clear; clf; clc;
format long;


%% TODO %%

% Fem korrekta decimaler på X!



%% Pre math

% Funktionen f och dess period T
f = @(t) exp(cos(pi/2*t))./(2+sin(pi*t)+0.5*cos(pi/2*t));
T = 4;

t = linspace(0,T,4711);

% Rita ut funktionskurvan
plot(t,f(t),'g'), hold on;

% Ungefärliga max-min-värden
maxX = [];
maxY = [];
minX = [];
minY = [];

%% Förbered Gyllenesnittetsökningen

% Snitt
rg = (sqrt(5)-1)/2; qg = 1-rg;

% Intervall där f är unimodal:
int = [0.0 1.0 ; 
       1.0 2.0 ;
       2.0 3.0 ;
       3.0 4.0];

%% Minpunkter

for i=1:2:3

    x1 = int(i,1)+qg*(int(i,2) - int(i,1));
    F1 = f(x1);
    x2 = int(i,1)+rg*(int(i,2) - int(i,1));
    F2 = f(x2);
    
    while abs(x1-x2) > 1e-8
        if F1 < F2
            int(i,2)=x2; 
            x2=x1; 
            F2=F1; 
            x1=int(i,1)+qg*(int(i,2)-int(i,1)); 
            F1=f(x1);
        end
        if F1 > F2
            int(i,1)=x1; 
            x1=x2; 
            F1=F2; 
            x2=int(i,1)+rg*(int(i,2)-int(i,1)); 
            F2=f(x2);
        end
    end
    minX = [minX int(i,1)];
    minY = [minY f(int(i,1))];
end

%% Maxpunkter

for i=2:2:4
    
    x1 = int(i,1)+qg*(int(i,2) - int(i,1));
    F1 = f(x1);
    x2 = int(i,1)+rg*(int(i,2) - int(i,1));
    F2 = f(x2);
    
    while abs(x1-x2) > 1e-8
        if F1 > F2
            int(i,2)=x2; 
            x2=x1; 
            F2=F1; 
            x1=int(i,1)+qg*(int(i,2)-int(i,1)); 
            F1=f(x1);
        end
        if F1 < F2
            int(i,1)=x1; 
            x1=x2; 
            F1=F2; 
            x2=int(i,1)+rg*(int(i,2)-int(i,1)); 
            F2=f(x2);
        end
        disp(abs(x1-x2));
    end
    disp('done');
    maxX = [maxX int(i,1)];
    maxY = [maxY f(int(i,1))];
    i = 0;
end

%% Markera max- och minpunkter i kurvan för f

plot(maxX,maxY,'pb');
plot(minX,minY,'pr');

axis equal

maxX
minX

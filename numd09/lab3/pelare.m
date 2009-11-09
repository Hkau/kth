format long

% "Glidkonstant" för uppgiften
G = [0.4, 0];
% Startradie för pelaren
R = 1;
% Starthöjd för pelaren
H = 4;

% Referensvolym, standardformel
real_volume = R^2*H*pi
% Referensarea = Area för cylinder
real_area = R*2*H*pi+2*R^2*pi

% Höjden för de ihoptryckta pelarna. h = 4 ger originalpelaren
hval = [4, 3.5, 3, 2.5, 2, 1.5, 1];

% Area-värden för de respektiva ihoptryckningarna kommer att lagras här. :)
area = [];

% Gå igenom alla "ihoptryckningar" av pelaren
for i = 1:length(hval)

	% Varje ihoptryckning ska räknas ut i en egen subplot.
	subplot(2, ceil(length(hval)/2), i)

	% Höjden för nuvarande ihoptryckning som vi räknar på
	h = hval(i)

	% Nuvarande radie, formel enligt spec.
	r = R*(1+G.*(sqrt(H/h)-1));

	% Startvärden, det verkar rimligt att vinkel ligger i första kvadranten
	angle = [0.2, 0.9];

	% "Tom" volymskillnadsmatris, sekantmetoden söker efter nollpunkt
	vol = [0, 0];

	% Ersätt med volymvärden för bezierkurvan
	for i = [1,2]

		% Vinkel för volymberäkning
		alpha = angle(i);

		% Motsvarande formel (2) under delen "Hitta styrpunkt för bezierkurva".
		z1 = (r(1)-r(2))*tan(alpha);
		% (3) under samma del, även Y-värde för beräkningarna
		dh = (h-z1)/2;
		% (4) under volymberäkningar
		z2 = dh/tan(alpha);

		% styrpunkt b (x, y), x = (5), y = (3)
		b = [r(1)+z2, dh];

		% funktion för uträkning av x-värden
		bezier_x = @(t) (1-t).^2*r(1) + 2*t.*(1-t)*b(1)+t.^2*r(2);
		% funktion för uträkning av z-värden
		bezier_z = @(t) (1-t).^2*0 + 2*t.*(1-t)*b(2)+t.^2*h;
		% derivata av denna
		bezier_z_deriv = @(t) (2*h-4*b(2))*t+2*b(2);

		% cylindersegment, analogt med pi*r^2*h
		pillar_slice = @(t) pi*bezier_x(t).^2*bezier_z_deriv(t);

		% integrera mellan 0,1, dvs hela kurvan. Volymskillnaden blir
		% nya volymen (integralen) minus startvolymen.
		vol(i) = quad(pillar_slice, 0, 1) - real_volume;
	end

	% Sekantmetoden för beräkning av korrekt vinkel. Körs tills senaste
	% uträknade volymen ligger tillräckligt nära den korrekta. Då har
	% vi hittat en bra approximation av vinkeln.
	while(abs(vol(2)) > 1e-10)
		% hitta nästa vinkel, motsvarande x-värde.
		alpha = angle(2)-((angle(2)-angle(1))/(vol(2)-vol(1)))*vol(2);

		% Gamla vinkeln flyttas
		angle(1) = angle(2);
		% Nya vinkeln blir senaste
		angle(2) = alpha;

		% Formel (2) till (4), precis som övre slinga.
		z1 = (r(1)-r(2))*tan(alpha);
		dh = (h-z1)/2;
		z2 = dh/tan(alpha);

		% Styrpunkt precis som övre slinga.
		b = [r(1)+z2, dh];

		% Samma funktioner som övre slinga
		bezier_x = @(t) (1-t).^2*r(1) + 2*t.*(1-t)*b(1)+t.^2*r(2);
		bezier_z = @(t) 2*t.*(1-t)*b(2)+t.^2*h;
		bezier_z_deriv = @(t) (2*h-4*b(2))*t+2*b(2);

		% Utklippning av pelare, precis som övre
		pillar_slice = @(t) pi*bezier_x(t).^2*bezier_z_deriv(t);

		% Senaste volymdiffen sparas som gamla.
		vol(1) = vol(2);
		% Den nya diffen räknas ut, som förut.
		vol(2) = quad(pillar_slice, 0, 1) - real_volume;
	end

	% skriv ut vinkeln för den ihopmosade pelaren
	angle

	% uträknat volymfel för pelaren (litet nog :))
	volume_error = vol(2)
	% Skriv ut styrpunkten
	styrpunkt = b


	% Areaberäkning:

	% Intressanta bezierkomponenter och derivator analogt med tidigare
	bezier_x = @(t) (1-t).^2*r(1) + 2*t.*(1-t)*b(1)+t.^2*r(2);
	bezier_x_deriv = @(t) (2*r(1)+2*r(2)-4*b(1))*t-2*r(1)+2*b(1);
	bezier_z = @(t) 2*t.*(1-t)*b(2)+t.^2*h;
	bezier_z_deriv = @(t) (2*h-4*b(2))*t+2*b(2);

	% Area = ring integrerad över sträcka(pyt sats.)
	bezier_area = @(t) 2*pi*bezier_x(t)*sqrt(bezier_x_deriv(t)^2+bezier_z_deriv(t)^2);

	% Integrera och lägg till nästa uträknade area
	area = [area; quad(bezier_area, 0, 1)+r(1)^2*pi+r(2)^2*pi];

	% Räkna ut och plotta bezierkurvans rotationsvolym (= pelaren)

	% tid, 24 värden per kurva
	t = linspace(0, 1, 24);

	% Räkna ut x och z-värden för bezierkurvan
	f_x = bezier_x(t);
	f_z = bezier_z(t);

	% 42 vinklar per varv, godtyckligt bra tal. :)
	a = linspace(0, 2*pi, 42)';

	% Räkna ut matris att plotta. Bezierkurvan roterad kring z-axeln
	plot_x = repmat(f_x, length(a), 1).*repmat(cos(a), 1,length(f_x));
	plot_y = repmat(f_x, length(a), 1).*repmat(sin(a), 1,length(f_x));
	plot_z = repmat(f_z, length(a), 1);

	hold on
	% Rita ut pelaren
	surf(plot_x, plot_y, plot_z)

	% och bezierkurvans styrpunkter
	% Våra uträknade z- och x-punkter
	x = [r(1) b(1) r(2)];
	z = [0 b(2) h];

	% och dess styrpunkt
	plot3(x, [0,0,0], z, '*-')
	hold off
	axis equal
end

% Se till att man får (rimlig) chans att beskåda överstående figurer.
pause

% Printa ut area, notera att första areavärdet svarar till h=4, och minskande, inte tvärt om
area 

% Plotta grafen i hela fönstret.
subplot(1, 1, 1)
% våra "x-värden" för minstakvadratmetoden är höjden av pelaren.
h = hval';

% Kända h-värden för anpassning av polynomet
f=[h.^-1 ones(length(h),1) h h.^2];
% Räkna ut (och visa) koefficienter med minstakvadratmetoden
c=f\area

% Funktion för arean
y=@(x) c(1)*x.^-1+c(2)*ones(length(x),1)+c(3)*x+c(4)*x.^2;
% Dess derivata
y_deriv = @(x) -c(1)*x.^-2+0+c(3)+2*c(4)*x;

% Startvärden för newton raphson
x0 = 2;
x1 = 1;

% Newton Raphson, med tolerans 1e-8
while(abs(x0-x1) > 1e-8)
	x0 = x1;
	% Vi söker punkten som motsvarar real_area, alltså måste vi kompensera för den i formeln
	x1 = x0 - (y(x0)-real_area)/y_deriv(x0);
	diff = x0-x1;
end

x_brytpunkt = x1

% X-värden för uppritning
x=[1:0.1:4]';

% Rita ut funktionen
plot(x,y(x))
hold on

% Rita ut ett sträck mellan den funna no
plot([x1, H], [y(x1), real_area], 'or-')

% Bra nog :) (storleksordningen 1e-15 trevligt nog)
diff_area = abs(real_area - y(x1))


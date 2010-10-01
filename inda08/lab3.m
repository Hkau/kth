%snygg tiltande skylt. Detta är en kommentar om den snygga skylten! :>
clf;
precision=600
v=0:2*pi/precision:2*pi;
x=cos(v);
y=sin(v);
hold on, axis equal
fill(x, y, 'r');

yel_x=x(precision*0.45:end*0.9);
yel_y=y(precision*0.45:end*0.9);

fill(0.8*yel_x, 0.8*yel_y, 'b');

fill(-0.8*yel_x, -0.8*yel_y, 'b');

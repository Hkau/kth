x = [1, 2, 3, 4];
y = [2, 5, 1, 4];

hold on
plot(x, y,'g*-')

px = [];
py = [];

for t = linspace(0, 1, 1000)
	tx = x;
	ty = y;
	while length(tx) > 1
		for i = 1:length(tx)-1
			tx(i) = tx(i)+t*(tx(i+1)-tx(i));
			ty(i) = ty(i)+t*(ty(i+1)-ty(i));
		end
		tx = tx(:, 1:end-1);
		ty = ty(:, 1:end-1);
	end
	px = [px tx(1)];
	py = [py ty(1)];
end

plot(px, py)


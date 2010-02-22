#include <stdio.h>

static inline int readint()
{
	register int num = 0;

	register char c = getchar();

	while(c < '0' || c > '9')
		c = getchar();

	do
	{
		num = num*10 + c - '0';
		c = getchar();
	} while(c >= '0' && c <= '9');

	return num;
}

int main()
{
	register int x, y, edges;
	x = readint();
	y = readint();
	edges = readint();
	//scanf("%d %d\n%d", &x, &y, &edges);
	register int verts = x+y;
	printf("%d\n%d %d\n", verts+2, verts+1, verts+2);

	printf("%d\n", edges+verts);

	register int e1, e2;
	register int i = edges;

	while(i--)
	{
		e1 = readint();
		e2 = readint();
		//scanf("%d %d", &e1, &e2);
		printf("%d %d 1\n", e1, e2);
	}

	for(i = x+1; i <= verts; i++)
	{
		printf("%d %d 1\n", i, verts+2);
	}

	for(i = 1; i <= x; ++i)
	{
		printf("%d %d 1\n", verts+1, i);
	}

	// inskickning till blackbox klar. :)

	fflush(stdout);

	printf("%d %d\n", x, y);

	register int flow;

	readint(); readint(); readint();
	flow = readint();
	//scanf("%d\n%d %d %d", &i, &i, &i, &flow); // släng vertexantalet & allt utom flöde!

	printf("%d\n", flow); // flöde = antalet kanter som är matchade = antalet kanter

	edges = readint();

	//scanf("%d", &edges); // antalet kanter

	while(edges--) // läs in så många kanter
	{
		e1 = readint();
		if(e1 > verts)
			return 0;
		e2 = readint();
		i = getchar();
		//i = readint();
		if(e2 > verts || i == '0')
			continue;
		//scanf("%d %d %d", &e1, &e2, &i);
		//if(e1 > verts || e2 > verts || i == 0)
		//	continue;
		printf("%d %d\n", e1, e2);
	}

	return 0;
}

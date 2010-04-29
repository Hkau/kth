#include <stdio.h>
#include <stdlib.h>

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
	//Indataformat:
	//Rad ett: tal V (antal hörn, V≥1)
	//Rad två: tal E (antal kanter, E≥0)
	//Rad tre: mål m (maxantal färger, m≥1)
	//En rad för varje kant (E stycken) med kantens ändpunkter (hörnen numreras från 1 till V) 

	int v = readint();
	int e = readint();
	int m = readint();

	//Utdataformat:
	//Rad ett: n(=V+2), s(=E+1) och k(=m+2) (antal roller n≥1, antal scener s≥1, och antal skådespelare k≥2).

	int n = v+3, s = e+2, k = m+2;

	if(k < 3)
		k = 3;

	printf("%d %d %d\n", n, s, k);

	//n rader: antal skådespelare som kan ansätta rollen + de skådespelare som kan ansätta rollen
	printf("1 1\n"); // första divrollen
	printf("1 2\n"); // andra divrollen
	printf("1 3\n"); // bluffroll för scen med divor

	int i;
	for(i = 4; i <= n; ++i)
	{
		printf("%d", k-2);
		int j;
		for(j = 3; j <= k; ++j)
			printf(" %d", j);
		putchar('\n');
	}

	//s rader: antal roller i scenen + de roller som ska vara med i scenen

	printf("2 1 3\n");
	printf("2 2 3\n");

	for(i = 3; i <= s; ++i)
	{
		printf("2 %d %d\n", readint() + 3, readint() + 3);
	}

	return 0;
}

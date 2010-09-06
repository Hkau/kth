#include <cstdio>

/*char set[]={
'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f',
'g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v',
'w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L',
'M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};*/

char set[] = "DEUSEXMACHINADEUSEXMACHINADEUSEXMACHINADEUSEXMACHINA";


int chars = sizeof(set)-1;

#define frame_width 12
#define frame_height 6

// 7x15 font
// 1240x886 pixels ish
// 180x60 characters
#define width 180-frame_width
#define height 64-frame_height


unsigned char image[width][height];

int main()
{
	unsigned int iterations = 1000+1; // 9 iterationer

	double x_size = 0.0015; //0.5
	double y_size = 0.001; //0.5

	double x_delta = x_size/(width);
	double y_delta = y_size/(height);

	double x_offset = -0.7447; //-1.6
	double y_offset = 0.1302; //-0.9

	unsigned int min_iteration = iterations;
	unsigned int max_iteration = 0;

	// Add half delta to search the center of the pixel, instead of the edge
	x_offset +=x_size/2;
	y_offset +=y_size/2;

	for(unsigned int i = 0; i < height; ++i)
	{
		for(unsigned int j = 0; j < width; ++j)
		{
			double x0 = x_offset + x_delta * j, x = x0;
			double y0 = y_offset + y_delta * i, y = y0;

			unsigned int iteration = 0;

			while (x*x + y*y < (2*2) && iteration < iterations) 
			{
				double xtemp = x*x - y*y + x0;
				y = 2.0*x*y + y0;

				x = xtemp;
				
				++iteration;
			}

			image[j][i] = iteration;

			if(iteration>max_iteration)
				max_iteration = iteration;
			if(iteration<min_iteration)
				min_iteration = iteration;
		}
	}

	//printf("(%d,%d) delta = (%e; %e)\nmax: %d\nmin: %d\n\n", width, height, x_delta, y_delta, max_iteration, min_iteration);

	#define xdist 8
	#define space(x) {for(int count = 0; count<x; ++count) printf(" "); }
	#define fill(x, c) {for(int count = 0; count<x; ++count) printf(c); }

	space(xdist+2); printf("im[z]\n");

	space(xdist); printf("^"); space(width-5); printf(".mandelbrot\n");

	space(xdist); printf("|\n");

	for(int i = 0; i < height; ++i)
	{
		if(i == 1)
		{
			if(y_offset+y_delta < 0)
				printf("%.4f -  ", y_offset+y_delta);
			else
				printf("%.5f -  ", y_offset+y_delta);
		}
		else if(i == height - 2)
		{
			if(y_offset+y_delta*(width-2) < 0)
				printf("%.4f -  ", y_offset+y_delta*(width-2));
			else
				printf("%.5f -  ", y_offset+y_delta*(width-2));
		}
		else
		{	space(xdist); printf("|  ");	}
		for(int j = 0; j < width; ++j)
		{
			if(image[j][i] == max_iteration || image[j][i]-min_iteration >= chars)
				printf(" ");
			else
				printf("%c", set[image[j][i]-min_iteration]);
		}
		printf("\n");
	}

	space(xdist); printf("|"); space(width+5); printf("re[z]\n");

	space(xdist); printf("+"); fill(3, "-"); printf("|"); fill(width-4, "-"); printf("|"); fill(3, "-"); printf(">\n");

	if(x_offset+x_delta<0)
		{space(xdist);}
	else
		{space(xdist+1);}
	printf("%.5f", x_offset+x_delta);

	if(x_offset+x_delta*(width-2)<0)
		{space(width-11);}
	else
		{space(width-10);}

	printf("%.5f", x_offset+x_delta*(width-2));

	return 0;
}

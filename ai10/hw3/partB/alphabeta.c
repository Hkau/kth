#include <stdio.h>
#include <string.h>

#define RIGHT 0
#define LEFT 1
#define STOP 2
#define FORWARD 3
const char *states[] = {"RIGHT", "LEFT", "STOP", "FORWARD"}; // labels for states 0...3

#define NONE 0
#define H1 1
#define H2 2
#define H3 3
#define H4 4
const char *observations[] = {"NONE", "H1", "H2", "H3", "H4"};

double tl_prob[4][4] = {
	{0.45, 0.09, 0.10, 0.36}, // RIGHT
	{0.08, 0.55, 0.10, 0.27}, // LEFT
	{0.08, 0.12, 0.60, 0.20}, // STOP
	{0.09, 0.09, 0.15, 0.67} // FORWARD
	};
double obs_prob[4][5] = {
	{-1.0, 0.60, 0.05, 0.15, 0.20},
	{-1.0, 0.12, 0.62, 0.10, 0.16},
	{-1.0, 0.15, 0.20, 0.63, 0.02},
	{-1.0, 0.01, 0.04, 0.25, 0.70}
	};

int main()
{
	unsigned int obs[] = {NONE, H1, H3, H3, H4, H2, H1, H3, H1, H1, H2, NONE};
	int t = sizeof(obs)/sizeof(obs[0]) - 2;
	int n = sizeof(states)/sizeof(states[0]);

	int i;
	for(i = 0; i < n; ++i)
		printf("state %d: %s\n", i, states[i]);
	puts("");

	for(i = 1; i <= t; ++i)
		printf("observation %2d: %s\n", i, observations[obs[i]]);

	puts("\n\nprobabilities:\n");

	for(i = 0; i < n; ++i)
	{
		int j;
		for(j = 0; j < n; ++j)
		{
			printf("%s->%s = %.02f\n", states[i], states[j], tl_prob[i][j]);
		}
		puts("");
	}

	for(i = 0; i < n; ++i)
	{
		int j;
		for(j = 1; j <= 4; ++j)
		{
			printf("%s::%s = %.02f\n", states[i], observations[j], obs_prob[i][j]);
		}
		puts("");
	}
	// let's go!

	double alpha[t+1][n];
	for(i = 0; i < n; ++i)
	{
		alpha[0][i] = 0.0;
	}

	alpha[0][RIGHT] = 1.0; // FIRST STATE = RIGHT;

	for(i = 1; i <= t; ++i)
	{
		int state;
		for(state = 0; state < n; ++state)
		{
			double sum = 0.0;

			int k;
			for(k = 0; k < n; ++k)
			{
				double val = alpha[i-1][k] * tl_prob[k][state];
				sum += val;
			}

			alpha[i][state] = sum * obs_prob[state][obs[i]];
		}
	}

	for(i = 0; i < n; ++i)
	{
		int j;
		for(j = 0; j <=t; ++j)
		{
			printf("%e ", alpha[j][i]);
		}
		puts("");
	}

	puts("");

	// beta
	double beta[t+1][n];
	for(i = 0; i < n; ++i)
	{
		beta[0][i] = 0; // doesn't matter, just for printing
		beta[t+1][i] = 1.0;
	}

	for(i = t; i > 0; --i)
	{
		int state;
		for(state = 0; state < n; ++state)
		{
			double sum = 0.0;

			int k;
			for(k = 0; k < n; ++k)
			{
				double val = beta[i+1][k] * tl_prob[k][state];
				sum += val;
			}

			beta[i][state] = sum * obs_prob[state][obs[i]];
		}
	}

	for(i = 0; i < n; ++i)
	{
		int j;
		for(j = 0; j <=t+1; ++j)
		{
			printf("%e ", beta[j][i]);
		}
		puts("");
	}

	for(i = 1; i <= t; ++i)
	{
		int j;
		int max_idx;
		double gamma = -1.0;
		for(j = 0; j < n; ++j)
		{
			double val = alpha[i][j] * beta[i][j];
			if(val > gamma)
			{
				max_idx = j;
				gamma = val;
			}
		}
		printf("state %d = %s : %e\n", i, states[max_idx], gamma);
	}

	return 0;
}


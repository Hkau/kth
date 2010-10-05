#include <stdio.h>

typedef struct
{
	double prob;
	int prev_idx;
}viterbi_node;

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
	unsigned int obs[] = {NONE, H1, H3, H3, H4, H2, H1, H3, H1, H1, H2};
	int t = sizeof(obs)/sizeof(obs[0]) - 1;
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
	viterbi_node chain[t+1][n];
	for(i = 0; i < n; ++i)
	{
		chain[0][i].prob = 0;
	}

	chain[0][RIGHT].prob = 1.0; // FIRST STATE = RIGHT;

	for(i = 1; i <= t; ++i)
	{
		int j;
		for(j = 0; j < n; ++j)
		{
			int k;
			int max_idx;
			double max_prob = -1.0;
			for(k = 0; k < n; ++k)
			{
				double val = chain[i-1][k].prob * tl_prob[k][j] * obs_prob[j][obs[i]];
				if(val > max_prob)
				{
					max_idx = k;
					max_prob = val;
				}
			}
			chain[i][j].prob = max_prob;
			chain[i][j].prev_idx = max_idx;
		}
	}

	int cur_idx;
	double goal_prob = -1.0;
	for(i = 0; i < n; ++i)
	{
		if(chain[t][i].prob > goal_prob)
		{
			cur_idx = i;
			goal_prob = chain[t][i].prob;
		}
	}

	puts("sequence in reverse order, cause reversing is not part of the task... and I'm lazy. :) Just read it in opposite order..");
	puts("----");
	for(i = t; i > 0; --i)
	{
		printf("chain[%d][%d].prob = %e :: ", i, cur_idx, chain[i][cur_idx].prob);
		printf("%d: %s\n", cur_idx, states[cur_idx]);
		cur_idx = chain[i][cur_idx].prev_idx;
	}

	// printf("Viterbi's motherfucker.\n");
	return 0;
}


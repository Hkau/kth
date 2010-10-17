#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STATE_COUNT 4
#define OBS_COUNT 10

char *states_str[] = {"RIGHT", "LEFT", "STOP", "FORWARD"};

char *obs_str[] = {"H1", "H2", "H3", "H4"};

int obs[] = {0, 2, 2, 3, 1, 0, 2, 0, 0, 1};

float start_p[] = {0.45, 0.09, 0.10, 0.36};

float transition_p[][4] = {
	{0.45, 0.09, 0.10, 0.36},
	{0.08, 0.55, 0.10, 0.27},
	{0.08, 0.12, 0.60, 0.20},
	{0.09, 0.09, 0.15, 0.67}
};

float obs_p[][4] = {
	{0.60, 0.05, 0.15, 0.20},
	{0.12, 0.62, 0.10, 0.16},
	{0.15, 0.20, 0.63, 0.02},
	{0.01, 0.04, 0.25, 0.70}
};

float V[OBS_COUNT][STATE_COUNT];
int path[STATE_COUNT][OBS_COUNT];
int path2[STATE_COUNT][OBS_COUNT];

int (*current_path)[STATE_COUNT][OBS_COUNT] = &path;
int (*new_path)[STATE_COUNT][OBS_COUNT] = &path2;

int *current_path_sizes;
int *new_path_sizes;

int main() {
	int i, t, x, max_state = 0;
	float max_prob = 0;
	
	current_path_sizes = malloc(STATE_COUNT * sizeof(int));
	new_path_sizes = malloc(STATE_COUNT * sizeof(int));
	
	memset(current_path_sizes, 0, STATE_COUNT * sizeof(int));
	memset(new_path_sizes, 0, STATE_COUNT * sizeof(int));

	for(i = 0; i < STATE_COUNT; ++i) {
		V[0][i] = start_p[i] * obs_p[i][obs[0]];
		*current_path[i][0] = i;
		++current_path_sizes[i];
	}
	
	for(t = 1; t < OBS_COUNT; ++t) {
		int n, *temp, (*temp2)[STATE_COUNT][OBS_COUNT];

		for(i = 0; i < STATE_COUNT; ++i) {
			max_prob = 0;
			for(n = 0; n < STATE_COUNT; ++n) {
				float prob = V[t - 1][n] * transition_p[n][i] * obs_p[i][obs[t]];

				if(prob > max_prob) {
					max_prob = prob;
					max_state = n;
				}
			}

			V[t][i] = max_prob;
			
			for(x = 0; x < current_path_sizes[max_state]; ++x) {
				*new_path[i][x] = *current_path[max_state][x];
				new_path_sizes[i] = current_path_sizes[max_state];
			}
			*new_path[i][new_path_sizes[i]++] = i;
		}

		// Swap
		temp = new_path_sizes;
		new_path_sizes = current_path_sizes;
		current_path_sizes = temp;
		
		temp2 = new_path;
		new_path = current_path;
		current_path = temp2;
	}
	
	max_prob = 0;
	for(i = 0; i < STATE_COUNT; ++i) {
		float prob = V[OBS_COUNT - 1][i];
		
		if(prob > max_prob) {
			max_prob = prob;
			max_state = i;
		}
	}

	printf("Observations:");
	for(x = 0; x < OBS_COUNT; ++x)
		printf(" %s", obs_str[obs[x]]);
	printf("\n");

	printf("Most likely sequence:");
	for(x = 0; x < current_path_sizes[max_state]; ++x)
		printf(" %s", states_str[*current_path[max_state][x]]);
	printf("\n");

	return 0;
}

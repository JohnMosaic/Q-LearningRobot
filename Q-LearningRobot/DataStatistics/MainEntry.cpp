#include <iostream>
#include "QLearningAlgorithmMin.h"
#include "CommonUtils.h"

int main(int argc, char** argv)
{
	if (argc != 7) print_usage();
	else
	{
		int matrix_dimension = atoi(argv[1]);
		int max_episode = atoi(argv[2]);
		double alpha = atof(argv[3]);
		double gamma = atof(argv[4]);
		int episode_interval = atoi(argv[5]);
		int data_groups = atoi(argv[6]);
		init_variables(matrix_dimension, max_episode, episode_interval, data_groups, alpha, gamma);
		training_model_cost(1);
		delete_heap_memory();
	}

	return 0;
}
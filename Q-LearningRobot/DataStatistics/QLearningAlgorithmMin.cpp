#include <iostream>
#include <string>
#include <time.h>
#include "CommonUtils.h"

int matrix_row, matrix_column, max_episode, episode_interval, episode;
int state_num, action_num, destination_state, data_groups;
int possible_action_num, possible_action_array_length;
int* possible_action_array;
int** matrix_index_array;
double** q_matrix, ** r_matrix;
double*** cost_matrix;
double alpha, gamma;

void init_cost_matrix()
{
	cost_matrix = new double** [episode];

	for (int i = 0; i < episode; i++)
	{
		cost_matrix[i] = new double* [data_groups + 1];

		for (int j = 0; j < data_groups + 1; j++)
		{
			cost_matrix[i][j] = new double[2];
			cost_matrix[i][j][0] = 0;
			cost_matrix[i][j][1] = 0;
		}
	}
}

void init_q_matrix()
{
	q_matrix = new double* [matrix_row];

	for (int i = 0; i < matrix_row; i++)
	{
		q_matrix[i] = new double[matrix_column];

		for (int j = 0; j < matrix_column; j++) q_matrix[i][j] = 0;
	}
}

void reset_q_matrix()
{
	for (int i = 0; i < matrix_row; i++)
	{
		for (int j = 0; j < matrix_column; j++) q_matrix[i][j] = 0;
	}
}

void init_random_r_matrix()
{
	matrix_index_array = new int* [matrix_row + 3];
	r_matrix = new double* [matrix_row];
	srand((unsigned)time(NULL));

	for (int i = 0; i < matrix_row; i++)
	{
		matrix_index_array[i] = new int[2];
		r_matrix[i] = new double[matrix_column];

		for (int j = 0; j < matrix_column; j++) r_matrix[i][j] = -1;

		int c = rand() % (matrix_column - 1);
		matrix_index_array[i][0] = c;
		r_matrix[i][c] = 0;

		while (true)
		{
			int c = rand() % (matrix_column - 1);

			if (r_matrix[i][c] == -1)
			{
				matrix_index_array[i][1] = c;
				r_matrix[i][c] = 0;
				break;
			}
		}
	}

	int r = rand() % matrix_row;
	matrix_index_array[matrix_row] = new int[1];
	matrix_index_array[matrix_row][0] = r;
	r_matrix[r][matrix_column - 1] = 100;
	int num = 1;

	while (true)
	{
		int r = rand() % matrix_row;

		if (r_matrix[r][matrix_column - 1] == -1)
		{
			matrix_index_array[matrix_row + num] = new int[1];
			matrix_index_array[matrix_row + num][0] = r;
			r_matrix[r][matrix_column - 1] = 100;

			if (++num == 3) break;
		}
	}
}

void init_variables(int _dimension, int _episode, int _interval, int _groups, double _alpha, double _gamma)
{
	matrix_row = _dimension;
	matrix_column = _dimension;
	state_num = _dimension;
	action_num = _dimension;
	destination_state = _dimension - 1;
	max_episode = _episode;
	episode_interval = _interval;
	data_groups = _groups;
	possible_action_array_length = 2 * _dimension;
	possible_action_array = new int[possible_action_array_length];
	episode = max_episode / episode_interval;
	alpha = _alpha;
	gamma = _gamma;
	init_cost_matrix();
	init_q_matrix();
	init_random_r_matrix();
}

void get_possible_action(int state)
{
	possible_action_num = 0;

	for (int i = 0; i < action_num; i++)
	{
		if (r_matrix[state][i] >= 0)
		{
			possible_action_array[possible_action_num] = i;
			possible_action_num++;
		}
	}
}

double get_max_q(double** q_matrix, int state)
{
	double temp_max = 0;

	for (int i = 0; i < action_num; i++)
	{
		if ((r_matrix[state][i] >= 0) && (q_matrix[state][i] > temp_max)) temp_max = q_matrix[state][i];
	}

	return temp_max;
}

int episode_iterator(int init_state, double** q_matrix)
{
	double max_q, q_before;
	int next_action, step = 1;

	while (true)
	{
		memset(possible_action_array, 0, possible_action_array_length * sizeof(int));
		get_possible_action(init_state);
		next_action = possible_action_array[rand() % possible_action_num];
		max_q = get_max_q(q_matrix, next_action);
		q_before = q_matrix[init_state][next_action];
		q_matrix[init_state][next_action] = (1 - alpha) * q_before + alpha * (r_matrix[init_state][next_action] + gamma * max_q);

		if (next_action == destination_state)
		{
			init_state = rand() % state_num;
			break;
		}
		else init_state = next_action;

		step++;
	}

	return init_state;
}

double calc_matrix_average_value()
{
	double d = 0.0;

	for (int i = 0; i < matrix_row; i++)
	{
		d += q_matrix[i][matrix_index_array[i][0]];
		d += q_matrix[i][matrix_index_array[i][1]];
	}

	d += q_matrix[matrix_index_array[matrix_row][0]][matrix_column - 1];
	d += q_matrix[matrix_index_array[matrix_row + 1][0]][matrix_column - 1];
	d += q_matrix[matrix_index_array[matrix_row + 2][0]][matrix_column - 1];

	return d / (2 * (double)matrix_row + 3);
}

void calc_cost_average_value()
{
	for (int i = 0; i < episode; i++)
	{
		double d_value = 0.0, d_time = 0.0;

		for (int j = 0; j < data_groups; j++)
		{
			d_value += cost_matrix[i][j][0];
			d_time += cost_matrix[i][j][1];
		}

		cost_matrix[i][data_groups][0] = d_value / data_groups;
		cost_matrix[i][data_groups][1] = d_time / data_groups;
	}
}

void training_model_cost(int init_state)
{
	srand((unsigned)time(NULL));

	for (int m = 0; m < data_groups + 1; m++)
	{
		if (m == data_groups)
		{
			calc_cost_average_value();
			break;
		}
		
		clock_t start_time = clock();
		int initial_state = init_state;

		for (int i = 0; i < max_episode; ++i)
		{
			initial_state = episode_iterator(initial_state, q_matrix);

			if ((i + 1) % episode_interval == 0)
			{
				clock_t end_time = clock();
				double elapsed_time = ((double)end_time - (double)start_time) / CLOCKS_PER_SEC;
				double averge_value = calc_matrix_average_value();
				int n = i / episode_interval;
				cost_matrix[n][m][0] = averge_value;
				cost_matrix[n][m][1] = elapsed_time;
			}
		}

		reset_q_matrix();
	}

	print_cost_matrix(cost_matrix, episode, data_groups, episode_interval);
}

void delete_heap_memory()
{
	for (int i = 0; i < episode; i++)
	{
		for (int j = 0; j < data_groups + 1; j++) delete[]cost_matrix[i][j];

		delete[]cost_matrix[i];
	}

	delete[]cost_matrix;
	delete[]possible_action_array;

	for (int i = 0; i < matrix_row; i++)
	{
		delete[]matrix_index_array[i];
		delete[]q_matrix[i];
		delete[]r_matrix[i];
	}
	
	delete[]matrix_index_array[matrix_row];
	delete[]matrix_index_array[matrix_row + 1];
	delete[]matrix_index_array[matrix_row + 2];
	delete[]matrix_index_array;
	delete[]q_matrix;
	delete[]r_matrix;
}

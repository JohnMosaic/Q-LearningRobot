#include <iostream>
#include <string>
#include <time.h>
#include "CommonUtils.h"
#include "SystemInfo.h"

using namespace std;

int matrix_row, matrix_column, max_episode;
int state_num, action_num, destination_state;
int* possible_action_array;
int possible_action_array_length;
int possible_action_num;
double** q_matrix, ** r_matrix;
double alpha, gamma;

void init_q_matrix()
{
	q_matrix = new double* [matrix_row];

	for (int i = 0; i < matrix_row; i++)
	{
		q_matrix[i] = new double[matrix_column];

		for (int j = 0; j < matrix_column; j++) q_matrix[i][j] = 0;
	}

	cout << "Initial matrix Q:" << endl;
	print_matrix(q_matrix, matrix_row, matrix_column);
}

void init_random_r_matrix()
{
	r_matrix = new double* [matrix_row];
	srand((unsigned)time(NULL));

	for (int i = 0; i < matrix_row; i++)
	{
		r_matrix[i] = new double[matrix_column];

		for (int j = 0; j < matrix_column; j++) r_matrix[i][j] = -1;

		int c = rand() % (matrix_column - 1);
		r_matrix[i][c] = 0;

		while (true)
		{
			int c = rand() % (matrix_column - 1);

			if (r_matrix[i][c] == -1)
			{
				r_matrix[i][c] = 0;
				break;
			}
		}
	}

	int r = rand() % matrix_row;
	r_matrix[r][matrix_column - 1] = 100;
	int num = 1;

	while (true)
	{
		int r = rand() % matrix_row;

		if (r_matrix[r][matrix_column - 1] == -1)
		{
			r_matrix[r][matrix_column - 1] = 100;

			if (++num == 3) break;
		}
	}

	cout << "Initial random matrix R:" << endl;
	print_matrix(r_matrix, matrix_row, matrix_column);
}

void init_variables(int _matrix_dimension, int _max_episode, double _alpha, double _gamma)
{
	matrix_row = _matrix_dimension;
	matrix_column = _matrix_dimension;
	state_num = _matrix_dimension;
	action_num = _matrix_dimension;
	destination_state = _matrix_dimension - 1;
	possible_action_array_length = 2 * _matrix_dimension;
	possible_action_array = new int[possible_action_array_length];
	max_episode = _max_episode;
	alpha = _alpha;
	gamma = _gamma;
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

int episode_iterator(int init_state, double** q_matrix, bool is_print_process)
{
	double max_q, q_before;
	int next_action, step = 1;

	if (is_print_process)
	{
		string s;

		while (true)
		{
			s.append(">> Step ").append(to_string(step)).append(": init state[").append(to_string(init_state));
			memset(possible_action_array, 0, possible_action_array_length * sizeof(int));
			get_possible_action(init_state);
			next_action = possible_action_array[rand() % possible_action_num];
			s.append("] -> next action[").append(to_string(next_action)).append("]\n");
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

		cout << s << endl;
	}
	else
	{
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
	}

	return init_state;
}

void training_model(int init_state, bool is_print_process)
{
	cout << "[INFO] Start training..." << endl;
	clock_t start_time = clock();
	int initial_state = init_state;
	srand((unsigned)time(NULL));

	if (is_print_process)
	{
		for (int i = 0; i < max_episode; ++i)
		{
			cout << "[INFO] Episode: " << (i + 1) << endl;
			initial_state = episode_iterator(initial_state, q_matrix, is_print_process);
			cout << "Updated matrix Q: " << endl;
			print_matrix(q_matrix, matrix_row, matrix_column);
		}
	}
	else
	{
		for (int i = 0; i < max_episode; ++i) initial_state = episode_iterator(initial_state, q_matrix, is_print_process);
	}

	clock_t end_time = clock();
	double elapsed_time = ((double)end_time - (double)start_time) / CLOCKS_PER_SEC;
	cout << "Convergent matrix Q:" << endl;
	print_matrix(q_matrix, matrix_row, matrix_column);
	cout << "[INFO] Training completed." << endl;
	get_system_info();
	cout << ">> " << max_episode << " episode of " << matrix_row << "-dimensional matrix takes " << elapsed_time << " seconds";

	if (is_print_process) cout << " with printing the training progresses." << endl;
	else cout << " without printing the training progresses." << endl;
}

int inference_best_action(int now_state)
{
	double temp_max_q = 0;
	int best_action = 0;

	for (int i = 0; i < action_num; i++)
	{
		if (q_matrix[now_state][i] > temp_max_q)
		{
			temp_max_q = q_matrix[now_state][i];
			best_action = i;
		}
	}

	return best_action;
}

void delete_heap_memory()
{
	delete[]possible_action_array;

	for (int i = 0; i < matrix_row; i++)
	{
		delete[]q_matrix[i];
		delete[]r_matrix[i];
	}

	delete[]q_matrix;
	delete[]r_matrix;
	cout << "[INFO] Delete requested heap memory." << endl;
}

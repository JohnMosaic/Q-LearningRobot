#include <iostream>
#include "QLearningAlgorithm.h"
#include "CommonUtils.h"

using namespace std;

int main(int argc, char** argv)
{
	if (argc != 6) print_usage();
	else
	{
		int matrix_dimension = atoi(argv[1]);
		int max_episode = atoi(argv[2]);
		double alpha = atof(argv[3]);
		double gamma = atof(argv[4]);
		int is_print_progress = atoi(argv[5]);
		int position;

		init_variables(matrix_dimension, max_episode, alpha, gamma);
		training_model(1, is_print_progress);

		while (true)
		{
			cout << ">> Input the location of the robot: [0-" << matrix_dimension - 1
				<< "], or number " << matrix_dimension << " to exit this program." << endl;
			cin >> position;

			if (position == matrix_dimension) break;
			else if (position < matrix_dimension && position >= 0)
			{
				cout << position << "->";

				while (true)
				{
					int best_action = inference_best_action(position);
					cout << best_action << "->";

					if (best_action == matrix_dimension - 1)
					{
						cout << "escaped." << endl;
						break;
					}
					else position = best_action;
				}
			}
		}

		delete_heap_memory();
	}

	return 0;
}

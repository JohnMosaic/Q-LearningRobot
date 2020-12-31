#include <iostream>
#include <string>

using namespace std;

void print_matrix(double** matrix, int rows, int columns)
{
	string s;

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			s.append(to_string(matrix[i][j]));
			s.append("\t");
		}

		s.append("\n");
	}

	cout << s << endl;
}

void print_usage()
{
	string s;
	s.append("+--------------------------------------------------------------------------------------+\n");
	s.append("|                                                                                      |\n");
	s.append("|   Q-LearningRobot: A demo of Q-learning algorithm implemented in C++    (2019534002) |\n");
	s.append("|                                                                                      |\n");
	s.append("+--------+--------+------------------+-------------+-------+-------+-------------------+\n");
	s.append("| Usage: |  this  | matrix_dimension | max_episode | alpha | gamma | is_print_progress |\n");
	s.append("+--------+--------+------------------+-------------+-------+-------+-------------------+\n");
	s.append("|  Type: | string |      integer     |   integer   |     double    |      boolean      |\n");
	s.append("+--------+--------+------------------+-------------+---------------+-------------------+\n");
	s.append("|        |   Without print progress  |       Q-LearningRobot.exe 6 1000 0.5 0.9 0      |\n");
	s.append("|        +---------------------------+-------------------------------------------------+\n");
	s.append("|  e.g.: |      With print progress  |       Q-LearningRobot.exe 6 1000 0.5 0.9 1      |\n");
	s.append("|        +---------------------------+-------------------------------------------------+\n");
	s.append("|        | Output progress log file  | Q-LearningRobot.exe 6 1000 0.5 0.9 1 >> qlr.log |\n");
	s.append("+--------+---------------------------+-------------------------------------------------+\n");
	s.append("|  Tips: |  Run this program from windows PowerShell (Recommended) or command prompt.  |\n");
	s.append("+--------+-----------------------------------------------------------------------------+");
	cout << s << endl;
}

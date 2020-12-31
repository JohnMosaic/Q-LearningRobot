#include <iostream>
#include <string>

using namespace std;

void print_cost_matrix(double*** matrix, int row, int column, int episode_interval)
{
	string s;
	s.append("[INFO] Data statistics of training cost.\n");
	s.append("VAL/T(s)\t");

	for (int m = 0; m < column; m++) s.append("Group-").append(to_string(m + 1)).append("\t");

	s.append("Average\n");

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < column + 2; j++)
		{
			if (j == 0) s.append("EP-").append(to_string((i + 1) * episode_interval)).append("\t");
			else if (j == column + 1)
			{
				s.append(to_string(matrix[i][j - 1][0])).append("/");
				s.append(to_string(matrix[i][j - 1][1])).append("\n");
			}
			else
			{
				s.append(to_string(matrix[i][j - 1][0])).append("/");
				s.append(to_string(matrix[i][j - 1][1])).append("\t");
			}
		}
	}

	cout << s << endl;
}

void print_usage()
{
	string s;
	s.append("+---------------------------------------------------------------------------+\n");
	s.append("|                                                                           |\n");
	s.append("|  A tiny tool of Q-learning algorithm data statistics implemented in C++.  |\n");
	s.append("|                                                                           |\n");
	s.append("+--------+--------+-----------+---------+-------+-------+----------+--------+\n");
	s.append("| Usage: |  this  |  matrix   |   max   | alpha | gamma |  episode |  data  |\n");
	s.append("|        |        | dimension | episode |       |       | interval | groups |\n");
	s.append("+--------+--------+-----------+---------+-------+-------+----------+--------+\n");
	s.append("|  Type: | string |       integer       |    double     |      integer      |\n");
	s.append("+--------+--------+---------------------+---------------+-------------------+\n");
	s.append("|        |  DataStatistics.exe 100 6000 0.5 0.9 200 5   |   training cost   |\n");
	s.append("|  e.g.: +----------------------------------------------+-------------------+\n");
	s.append("|        |  DataStatistics.exe 100 6000 0.5 0.9 200 5 >> training_cost.log  |\n");
	s.append("+--------+------------------------------------------------------------------+\n");
	s.append("|  Tips: |  Run this tool from PowerShell (Recommended) or command prompt.  |\n");
	s.append("+--------+------------------------------------------------------------------+");
	cout << s << endl;
}

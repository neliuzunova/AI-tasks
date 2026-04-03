//Neli Uzunova 81786
#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <algorithm>
#include <random>

using namespace std;
using namespace std::chrono;

int N;
vector<int> queens;      // row of Queen in every column
vector<int> queensOnRow; // queens per row
vector<int> diagonal1;   // num queend per main diagonal
vector<int> diagonal2;   // num queend per secondary diagonal

void initializeQueens()
{

	for (int i = 0; i < N; i++)
	{
		queens.push_back(i);
	}
	std::shuffle(queens.begin(), queens.end(), std::random_device());
	// shuffleing vector from 0 to N-1 for queens row positions

	queensOnRow.clear();
	queensOnRow.resize(N, 0);
	diagonal1.clear();
	diagonal1.resize(2 * N - 1, 0);
	diagonal2.clear();
	diagonal2.resize(2 * N - 1, 0);

	for (int i = 0; i < N; i++)
	{
		queensOnRow[queens[i]]++;
		diagonal1[i - queens[i] + N - 1]++;
		diagonal2[i + queens[i]]++;
	}

}

int getColWithQueenWithMaxConf()
{
	vector<int> queenConflicts;

	for (int i = 0; i < N; i++)
	{
		int result = queensOnRow[queens[i]] + diagonal1[i - queens[i] + N - 1] + diagonal2[i + queens[i]] - 3;
		//row = queens[i], col = i
		//number of conflicts in row i + number of conflicts in main diagonal for i + number of conflicts in sec diagonal for i
		
		queenConflicts.push_back(result);
	}

	vector<int> maxCols;
	auto MAX = std::max_element(queenConflicts.begin(), queenConflicts.end());

	if (*MAX == 0)
	{
		return -1;
	}

	for (int i = 0; i < N; i++)
	{
		if (queenConflicts[i] == *MAX)
		{
			maxCols.push_back(i);
		}
	}

	return maxCols[rand() % maxCols.size()];
	//we return a random of the minimal rows so we dont get stuck
}

int getRowWithMinConflicts(int column)
{
	vector<int> rowConflicts;
	for (int i = 0; i < N; i++)
	{
		int result = queensOnRow[i] + diagonal1[(column - i) + N - 1] + diagonal2[i + column];
		// row = i, col = column

		if (queens[column] == i)
		{
			result-= 3;
		}
		rowConflicts.push_back(result);
	}

	vector<int> minRows;
	auto MIN = std::min_element(rowConflicts.begin(), rowConflicts.end());

	for (int i = 0; i < N; i++)
	{
		if (rowConflicts[i] == *MIN)
		{
			minRows.push_back(i);
		}
	}

	return minRows[rand() % minRows.size()]; 
	//we return a random of the minimal rows so we dont get stuck
}

void printBoard()
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (i == queens[j])  //if it's a queen
				cout << "* ";
			else
				cout << "_ ";
		}
		cout << endl;
	}
	cout << endl;
}

void solve()
{
	initializeQueens();
	int i = 0, row, col;
	
	while (i++ <= 3*N)
	{

		col = getColWithQueenWithMaxConf();
		if (col == -1)  //ako nqma konflikti sme gotovi
		{
			return;
		}
		row = getRowWithMinConflicts(col);

		queensOnRow[queens[col]]--;
		diagonal1[col - queens[col] + N - 1]--;
		diagonal2[col + queens[col]]--;

		queens[col] = row;

		queensOnRow[row]++;
		diagonal1[col - row + N - 1]++;
		diagonal2[col + row]++;
		
	}

	//if we get stuck
	solve();
}

int main()
{
	srand((unsigned)time(0));

	N = 0;
	while (N < 4)
	{
		cout << "N = ";
		cin >> N;
	}

	auto start = steady_clock::now();

	solve();

	auto end = steady_clock::now();
	duration<double> elapsed_seconds = end - start;
	std::cout << "elapsed time: " << elapsed_seconds.count() << "s \n";

	if ( N <= 100 )
	{
		printBoard();
	}
}
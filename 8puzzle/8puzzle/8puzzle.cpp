//Neli Uzunova 81786
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cstring>
#include <stack>
#include <chrono>

using namespace std::chrono;

int puzzleSize;
bool foundSolution = false;

struct Puzzle
{
	int board[10][10] = { 0 };
	int emptyRow;
	int emptyCol;
	int distance = 0;

	bool operator==(Puzzle& puzzle)
	{
		for (int i = 0; i < puzzleSize; i++)
		{
			for (int j = 0; j < puzzleSize; j++)
			{
				if (this->board[i][j] != puzzle.board[i][j])
				{
					return false;
				}
			}
		}
		return true;
	}

	int manhattanDist(Puzzle* goal)
	{
		int manhattan = 0;
		for (int i = 0; i < puzzleSize; i++)
		{
			for (int j = 0; j < puzzleSize; j++)
			{
				if (board[i][j] != 0)
				{
					for (int row = 0; row < puzzleSize; row++)
					{
						for (int col = 0; col < puzzleSize; col++)
						{
							if (goal->board[row][col] == board[i][j])
							{
								manhattan += abs(row - i) + abs(col - j);
								goto p1;
							}
						}
					}
				p1:;
				}
			}
		}
		return manhattan;
	}
	
};

void printPuzzle(Puzzle& puz)
{
	for (int i = 0; i < puzzleSize; i++)
	{
		for (int j = 0; j < puzzleSize; j++)
		{
			std::cout << puz.board[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

bool isSolvable(Puzzle& puz)
{
	std::vector<int> rowMajorOrder;
	int invertions = 0;
	for (int i = 0; i < puzzleSize; i++)
	{
		for (int j = 0; j < puzzleSize; j++)
		{
			if (puz.board[i][j] != 0)
			{
				rowMajorOrder.push_back(puz.board[i][j]);
			}
		}
	}

	for (int i = 0; i < rowMajorOrder.size() - 1; i++)
	{
		for (int j = i + 1; j < rowMajorOrder.size(); j++)
		{
			if (rowMajorOrder[j] < rowMajorOrder[i] )
				invertions++;
		}
	}

	bool isEvenInvertions = invertions % 2 == 0;

	if (puzzleSize % 2 != 0)
	{
		return isEvenInvertions;
	}
	if (puz.emptyRow % 2 == 0)
	{
		return !isEvenInvertions;
	}
	return isEvenInvertions;

}

Puzzle generateGoal(int emptyPosition)
{
	Puzzle goal;
	int num = 1;
	if (emptyPosition == -1)
	{
		emptyPosition = (puzzleSize * puzzleSize);
	}
	for (int i = 0; i < puzzleSize; i++)
	{
		for (int j = 0; j < puzzleSize; j++)
		{
			if (num < emptyPosition)
				goal.board[i][j] = num;
			if (num == emptyPosition)
				goal.board[i][j] = 0;
			if (num > emptyPosition)
				goal.board[i][j] = num - 1;

			num++;
		}
	}

	return goal;
}

bool isVisited(Puzzle* currentPuzzle, std::vector<Puzzle>* visited)
{
	for (int i = 0; i < visited->size(); i++)
	{
		if (*currentPuzzle == (visited->at(i)))
		{
			return true;
		}
	}
	return false;
}

int solve(Puzzle* currentPuzzle, Puzzle* goal, int treshold, std::vector<std::string> path, std::vector<Puzzle>* visited, std::vector<int>* kidsEvristics)
{
	if (foundSolution)
	{
		return true;
	}
	auto manh = currentPuzzle->manhattanDist(goal);
	if (manh == 0)
	{
		std::cout << currentPuzzle->distance << "\n";
		foundSolution = true;
		path.erase(path.begin());
		for (auto i : path) {
			std::cout << i << "\n";
		}
		// we found the goal
		return true;
	}
	auto evr = currentPuzzle->distance + manh;
	if (treshold < evr)
	{
		kidsEvristics->push_back(evr);
		return false;
	}
	if (treshold == 0)
	{
		return false;
	}
	/*if (isVisited(currentPuzzle, visited))
	{
		return false;
	}*/

	visited->push_back(*currentPuzzle);

	if (currentPuzzle->emptyRow < puzzleSize - 1 && path.back() != "down ")
	{
		//UP
		std::swap(currentPuzzle->board[currentPuzzle->emptyRow][currentPuzzle->emptyCol], currentPuzzle->board[currentPuzzle->emptyRow + 1][currentPuzzle->emptyCol]);
		currentPuzzle->emptyRow += 1;
		currentPuzzle->distance++;
		path.push_back("up ");
		solve(currentPuzzle, goal, treshold, path, visited, kidsEvristics);
		std::swap(currentPuzzle->board[currentPuzzle->emptyRow][currentPuzzle->emptyCol], currentPuzzle->board[currentPuzzle->emptyRow - 1][currentPuzzle->emptyCol]);
		currentPuzzle->emptyRow -= 1;
		currentPuzzle->distance--;
		path.pop_back();
	}

	if (currentPuzzle->emptyRow > 0 && path.back() != "up ")
	{
		//DOWN
		std::swap(currentPuzzle->board[currentPuzzle->emptyRow][currentPuzzle->emptyCol], currentPuzzle->board[currentPuzzle->emptyRow - 1][currentPuzzle->emptyCol]);
		currentPuzzle->emptyRow -= 1;
		currentPuzzle->distance++;
		path.push_back("down ");
		solve(currentPuzzle, goal, treshold, path, visited, kidsEvristics);
		std::swap(currentPuzzle->board[currentPuzzle->emptyRow][currentPuzzle->emptyCol], currentPuzzle->board[currentPuzzle->emptyRow + 1][currentPuzzle->emptyCol]);
		currentPuzzle->emptyRow += 1;
		currentPuzzle->distance--;
		path.pop_back();
	}

	if (currentPuzzle->emptyCol < puzzleSize - 1 && path.back() != "right ")
	{
		//LEFT
		std::swap(currentPuzzle->board[currentPuzzle->emptyRow][currentPuzzle->emptyCol], currentPuzzle->board[currentPuzzle->emptyRow][currentPuzzle->emptyCol + 1]);
		currentPuzzle->emptyCol += 1;
		currentPuzzle->distance++;
		path.push_back("left ");
		solve(currentPuzzle, goal, treshold, path, visited, kidsEvristics);
		std::swap(currentPuzzle->board[currentPuzzle->emptyRow][currentPuzzle->emptyCol], currentPuzzle->board[currentPuzzle->emptyRow][currentPuzzle->emptyCol - 1]);
		currentPuzzle->emptyCol -= 1;
		currentPuzzle->distance--;
		path.pop_back();
	}

	if (currentPuzzle->emptyCol > 0 && path.back() != "left ")
	{
		//IGHT
		std::swap(currentPuzzle->board[currentPuzzle->emptyRow][currentPuzzle->emptyCol], currentPuzzle->board[currentPuzzle->emptyRow][currentPuzzle->emptyCol - 1]);
		currentPuzzle->emptyCol -= 1;
		currentPuzzle->distance++;
		path.push_back("right ");
		solve(currentPuzzle, goal, treshold, path, visited, kidsEvristics);
		std::swap(currentPuzzle->board[currentPuzzle->emptyRow][currentPuzzle->emptyCol], currentPuzzle->board[currentPuzzle->emptyRow][currentPuzzle->emptyCol + 1]);
		currentPuzzle->emptyCol += 1;
		currentPuzzle->distance--;
		path.pop_back();
	}
}

int main()
{
	int N, emptyPosition;
	std::cout << "Size = ";
	std::cin >> N;
	std::cout << "Empty position = ";
	std::cin >> emptyPosition;

	puzzleSize = sqrt(N + 1);

	Puzzle initialPuzzle;

	for (int i = 0; i < puzzleSize; i++)
	{
		for (int j = 0; j < puzzleSize; j++)
		{
			std::cin >> initialPuzzle.board[i][j];
			if (initialPuzzle.board[i][j] == 0)
			{
				initialPuzzle.emptyRow = i;
				initialPuzzle.emptyCol = j;
			}
		}
	}

	if (emptyPosition == -1 && !isSolvable(initialPuzzle))
	{
		std::cout << "Board is not solvable!";
		return 0;
	}

	auto start = steady_clock::now();

	Puzzle goal = generateGoal(emptyPosition);

	initialPuzzle.manhattanDist(&goal);
	std::vector<std::string> path ;
	path.push_back("");
	int depth = initialPuzzle.manhattanDist(&goal);

	while (!foundSolution)
	{
		std::vector<Puzzle> visited;
		std::vector<int> kidsEvristics;
		solve(&initialPuzzle, &goal, depth, path, &visited, &kidsEvristics);
		//treshold optimization
		auto min_value = *std::min_element(kidsEvristics.begin(), kidsEvristics.end());
		depth = min_value;
	}

	auto end = steady_clock::now();
	duration<double> elapsed_seconds = end - start;
	std::cout << "elapsed time: " << elapsed_seconds.count() << "s \n";

	return 0;
} 

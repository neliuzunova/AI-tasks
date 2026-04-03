//Neli Uzunova 81786
#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;

int winner;
// the player   is playing with 0 = X
// the computer is playing with 1 = O -> so he searches for max value

class Board {
public:
	int board[3][3];
	int value, depth;
	Board* bestComputerMove;
	bool X_or_O;

	Board()
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				board[i][j] = -100;
			}
		}
		depth = 0;
	}

	Board(int other[3][3], int otherdepth)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				board[i][j] = other[i][j];
			}
		}
		this->depth = otherdepth;
	}

	Board& operator=(const Board& other)
	{
		if (this != &other)
		{
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					board[i][j] = other.board[i][j];
				}
			}
		}
		this->depth = other.depth;
		this->X_or_O = other.X_or_O;
		this->value = other.value;
		this->bestComputerMove = other.bestComputerMove;
		return *this;
	}

	void setComputerMove(Board* otherBoard)
	{
		bestComputerMove = otherBoard;
		X_or_O = true;
	}

	int isTerminal()
	{
		// -2 it is not terminal state
		// 1 for Computer
		// 0 for draw
		// -1 for Player

		// we have finite board with only 1 and 0 so we can calculate the sums for:
		int sums[8] = { 0 };
		bool hasEmpty = false;
		for (int i = 0; i < 3; i++)
		{
			//rows
			sums[0] += board[0][i];
			sums[1] += board[1][i];
			sums[2] += board[2][i];
			//cols
			sums[3] += board[i][0];
			sums[4] += board[i][1];
			sums[5] += board[i][2];
			//diagonals
			sums[6] += board[i][i];
			sums[7] += board[2 - i][i];
		}
		for (int i = 0; i < 8; i++)
		{
			if (sums[i] == 0)
			{
				winner = -1;
				value = -10 + depth;
				return -1;
				//Computer
			}
			else if (sums[i] == 3)
			{
				winner = 1;
				value = 10 - depth;
				return 1;
				//Player
			}
			if (sums[i] < -50)
			{
				hasEmpty = true;
			}
		}
		if (hasEmpty == false)
		{
			winner = 0;
			value = 0;
			return 0;
			//Draw
		}
		return -2;
		//Not terminal
	}

	void print()
	{
		cout << "     1   2   3  " << endl;
		cout << "   +---+---+---+" << endl;
		for (int i = 0; i < 3; i++)
		{
			cout << " " << i + 1 << " | ";
			for (int j = 0; j < 3; j++)
			{
				if (board[i][j] == -100)
				{
					cout << " " << " | ";
				}
				else
				{
					if (board[i][j] == 0)
					{
						cout << "X" << " | ";
					}
					else
					{
						cout << "O" << " | ";
					}
				}
			}
			cout << endl << "   +---+---+---+" << endl;
		}
		cout << endl;
	}

	void PlayerMove()
	{
		int x, y;

		do {
			cout << "Insert position: " << endl;
			cin >> x >> y;

			if (x > 3 || y > 3 || x < 1 || y < 1 || board[x - 1][y - 1] == 0 || board[x - 1][y - 1] == 1)
			{
				cout << "This position is invalid, please choose another one" << endl;
			}
		} while (x > 3 || y > 3 || x < 1 || y < 1 || board[x - 1][y - 1] == 0 || board[x - 1][y - 1] == 1);

		board[x - 1][y - 1] = 0;
		// the player is playing with 0 = X
		// the computer - with 1 = O
		print();
		depth++;
	}

	vector<Board> getSuccessors()
	{
		vector<Board> successors;

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (board[i][j] == -100)
				{
					Board newSuccessor(this->board, this->depth + 1);
					newSuccessor.board[i][j] = X_or_O;
					successors.push_back(newSuccessor);
				}
			}
		}

		return successors;
	}
};

int MinValue(Board&, int, int);

int MaxValue(Board& state, int alpha, int beta)
{
	if (state.isTerminal() != -2) // not terminal = -2
	{
		return state.value;
	}

	state.value = -1000;
	state.X_or_O = true;
	vector<Board> successors = state.getSuccessors();
	int newValue;

	for (Board current : successors)
	{
		newValue = MinValue(current, alpha, beta);
		if (state.value < newValue)
		{
			state.value = newValue;
			state.setComputerMove(new Board(current.board, current.depth));
		}
		if (state.value >= beta) //otsichane
		{
			return state.value;
		}
		alpha = max(alpha, state.value);
	}
	return state.value;
}

int MinValue(Board& state, int alpha, int beta)
{

	if (state.isTerminal() != -2) // not terminal = -2
	{
		return state.value;
	}

	state.value = 1000;
	state.X_or_O = false;
	vector<Board> successors = state.getSuccessors();

	for (Board current : successors)
	{
		state.value = min(state.value, MaxValue(current, alpha, beta));
		if (state.value <= alpha) // otsichane
		{
			return state.value;
		}
		beta = min(beta, state.value);
	}
	return state.value;
}

void solution()
{
	string answer;
	cout << "Do you want to play first? - yes or no" << endl;
	cin >> answer;

	Board current;
	int Alpha = -1000;
	int Beta = 1000;

	current.print();
	if (answer == "yes")
	{
		current.PlayerMove();
		current.X_or_O = false;
	}

	while (current.isTerminal() == -2)
	{
		MaxValue(current, Alpha, Beta);
		current = Board(current.bestComputerMove->board, current.depth + 1);
		cout << "Computer move:" << endl;
		current.print();

		if (current.isTerminal() == -2)
		{
			current.PlayerMove();
		}
	}

	// 1 for Computer
	// 0 for draw
	// -1 for Player
	if (winner > 0)
	{
		cout << "- you lose  -" << endl;
	}
	else if (winner == 0)
	{
		cout << "- it's draw -" << endl;
	}
	else
	{
		cout << "- YOU WIN -" << endl;
	}
}

int main()
{
	solution();
	return 0;
}
//Neli Uzunova 81786
#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;
/*
Votes:
yes = 0 = democrat
no = 1 = republican
? = 2

Number of Instances: 435 (267 democrats, 168 republicans)
43 rows in one test group 435:10
*/

vector<vector<int>> allData;
vector<vector<int>> democratData;
vector<vector<int>> republicanData;
vector<vector<double>> democratTable;
vector<vector<double>> republicanTable;
double democratCount;
double republicanCount;

void learn(int index)
{
	democratTable.clear();
	democratTable.resize(16, { 1,1,1 });
	republicanTable.clear();
	republicanTable.resize(16, { 1,1,1 });
	democratCount = 3;
	republicanCount = 3;
	//zero problem solving - laplas smoothing - all data starting from 1

	for (int i = 0; i < allData.size(); i++)
	{
		if (i == index * 40)
			i += 40; // all data without the test group

		for (int j = 0; j < 16; j++)
		{
			if (allData[i][0] == 0) // if democrat
				democratTable[j][allData[i][j]]++;
			else
				republicanTable[j][allData[i][j]]++;
		}

		if (allData[i][0] == 0)
			democratCount++;
		else
			republicanCount++;
	}

	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			democratTable[i][j] = log(democratTable[i][j] / (democratCount));
			republicanTable[i][j] = log(republicanTable[i][j] / (republicanCount));

			//democratTable[i][j] = log(democratTable[i][j]) - log(democratCount + 16);
			//republicanTable[i][j] = log(republicanTable[i][j]) -log(republicanCount+ 16);
		}
	}

	democratCount = log(democratCount / (democratCount + republicanCount)); // 0.625
	republicanCount = log(republicanCount / (democratCount + republicanCount)); // 0.375
}

double test(int index)
{
	double democratProbability;
	double republicanProbability;
	double correctAnswers = 0;

	for (int i = index * 40; i < ((index + 1) * 40); i++)
	{
		democratProbability = democratCount;
		republicanProbability = republicanCount;

		for (int j = 1; j < 17; j++)
		{
			democratProbability += democratTable[j - 1][allData[i][j]];
			republicanProbability += republicanTable[j - 1][allData[i][j]];
			// log probability
		}

		//check if probability is correct
		if (democratProbability > republicanProbability && allData[i][0] == 0) // if its democrat
		{
			correctAnswers++;
		}
		else if (democratProbability < republicanProbability && allData[i][0] == 1) // if its republican
		{
			correctAnswers++;
		}
	}
	return 100.0 * (correctAnswers / 40.0);
}

void saveLine(string line)
{
	vector<int> currentLine;
	int index = 0;
	if (line[0] == 'd') // democrat,
	{
		currentLine.push_back(0);
		index = 9;
	}
	else if (line[0] == 'r') //republican,
	{
		currentLine.push_back(1);
		index = 11;
	}

	for (int i = index; i < line.size(); i += 2)
	{
		switch (line[i])
		{
		case 'y':
		{
			currentLine.push_back(0);
			break;
		}
		case 'n':
		{
			currentLine.push_back(1);
			break;
		}
		case '?':
		{
			currentLine.push_back(2);
			break;
		}
		}
	}
	//allData.push_back(currentLine);
	if (line[0] == 'd')
	{
		democratData.push_back(currentLine);
	}
	else if (line[0] == 'r')
	{
		republicanData.push_back(currentLine);
	}
}

void readFile()
{
	ifstream file("house-votes-84.data");
	string line;

	while (getline(file, line))
	{
		saveLine(line);
	}

	file.close();
}

void betterShuffle()
{
	random_shuffle(republicanData.begin(), republicanData.end());
	random_shuffle(democratData.begin(), democratData.end());

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			allData.push_back(republicanData[i * 15 + j]);
		}
		for (int k = 0; k < 25; k++)
		{
			allData.push_back(democratData[i * 25 + k]);
		}
	}
	allData.push_back(democratData[250]);
	//allData.push_back(democratData[251]);
	//allData.push_back(democratData[252]);
	/*
	for (int i = 153; i < 168; i++)
	{
		allData.push_back(republicanData[i]);
	}
	for (int i = 234; i < 267; i++)
	{
		allData.push_back(democratData[i]);
	}
	*/
}

int main()
{
	srand((unsigned)time(0));
	readFile();

	//idea for optimizatin is for better shuffle;
	betterShuffle();
	//random_shuffle(allData.begin(), allData.end());

	double testAccuracy = 0;
	double averageAccuracy = 0;

	for (int i = 0; i < 10; i++)
	{
		learn(i);
		testAccuracy = test(i);
		cout << i + 1 << ": " << testAccuracy << endl;
		averageAccuracy += testAccuracy;
	}

	cout << "Accuracy: " << averageAccuracy / 10.0 << std::endl;
}
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int N;
float distances[110][110] = { 0 };
vector<string> names;

class City
{
public:
	int id;
	int x;
	int y;

	City()
	{
		x = -1;
		y = -1;
		id = -1;
	}

	City(int x, int y, int id)
	{
		this->x = x;
		this->y = y;
		this->id = id;
	}
	
	float distance(City& to) 
	{
		return distances[id][to.id]; 
	}

	bool operator==(City other) const
	{
		return (x == other.x) && (y == other.y);
	}
};

vector<City> cities;

class Path
{
public:
	vector<City> path;
	float fitness;

	Path()
	{
		fitness = -1;
	}

	Path(vector<City> cities)
	{
		this->path = cities;
		fitness = 0;
		calcFitness();
	}

	void print()
	{
		for (int i = 0; i < N; i++)
		{
			cout << names[path[i].id] << " -> ";
			//cout << path[i].id << " " ;
		}
		cout << fitness << endl;
	}

	void calcFitness()
	{
		fitness = 0;
		for (int i = 1; i < N; i++)
		{
			fitness += distances[path[i-1].id-1][path[i].id-1]; // adds distance from previous City to the next
		}
	}

	bool operator< (Path other)
	{
		return fitness < other.fitness;
	}
};

vector<Path> paths;

void calcDistances()
{
	// calc distances between cities
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			float xDist, yDist;
			xDist = abs(cities[i].x - cities[j].x);
			yDist = abs(cities[i].y - cities[j].y);
			distances[i][j] = sqrt((xDist * xDist) + (yDist * yDist));
		}
	}
	/*
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			cout << distances[i][j] << " ";
		}
		cout << endl;
	}
	*/
}

void randomCoordinats()
{
	for (int i = 1; i <= N; i++)
	{
		cities.push_back(City(rand() % 10000, rand() % 10000, i));
		//cout << cities[i].x << " " << cities[i].y << " " << cities[i].id << endl;	
	}
	calcDistances();
}

void moodleExample()
{
	// from 1 to 12 or 12 to 1 will be the correct path
	cities.push_back(City(0.000190032, -0.000285946, 1));
	cities.push_back(City(383.458, -0.000608756,    11));
	cities.push_back(City(-27.0206, -282.758,        5));
	cities.push_back(City(335.751, -269.577,         8));
	cities.push_back(City(69.4331, -246.78,          4));
	cities.push_back(City(168.521, 31.4012,          2));
	cities.push_back(City(320.35, -160.9,            9));
	cities.push_back(City(179.933, -318.031,         6));
	cities.push_back(City(492.671, -131.563,        12));
	cities.push_back(City(112.198, -110.561,         3));
	cities.push_back(City(306.32, -108.09,          10));
	cities.push_back(City(217.343, -447.089,         7));

	calcDistances();

	names.push_back(" "); // for 0
	names.push_back("Aberystwyth");
	names.push_back("Brighton   ");
	names.push_back("Edinburgh  ");
	names.push_back("Exeter     ");
	names.push_back("Glasgow    ");
	names.push_back("Inverness  ");
	names.push_back("Liverpool  ");
	names.push_back("London     ");
	names.push_back("Newcastle  ");
	names.push_back("Nottingham ");
	names.push_back("Oxford     ");
	names.push_back("Stratford  ");
}


bool isMember(City A, vector<City> B)
{
	for (int i = 0; i < B.size(); i++)
	{
		if (A == B[i])
		{
			return true;
		}
	}
	return false;
}

Path crossover(Path firstParent, Path secondParent)
{
	//two-point crossover
	vector<City> child(N);
	int a = N / 3 + 1;
	int b = N * 2 / 3;

	for (int i = a; i <= b; i++)
	{
		child[i] = firstParent.path[i];
	}
	int cursor = b + 1;
	for (int i = b + 1; i < N; i++)
	{
		if (!isMember(secondParent.path[i], child))
		{
			child[cursor] = secondParent.path[i];
			cursor++;
			if (cursor == N)
			{
				cursor = 0;
			}
		}
	}

	for (int i = 0; i < N; i++)
	{
		if (!isMember(secondParent.path[i], child))
		{
			child[cursor] = secondParent.path[i];
			cursor++;
			if (cursor == N)
			{
				cursor = 0;
			}
		}
	}
	Path(child).calcFitness(); 
	return Path(child);
}

Path mutate(Path path)
{
	// random swap of two cities
	int a = rand() % N;
	int b = rand() % N;

	//path.print();
	Path mutated = path;
	swap(mutated.path[a], mutated.path[b]);
	mutated.calcFitness();
	//mutated.print();
	return mutated;
}

void selection()
{
	vector<Path> parentsPopulation = paths;
	//leaving 200 best paths
	paths.resize(200);
	// adding 50 random "bad" paths
	for (int i = 0; i < 50; i++ )
	{
		paths.push_back(parentsPopulation[rand() % 800 + 200]);
	}
	parentsPopulation.clear();
}

void addNewPaths()
{
	int cursor = 250;
	for (int i = 1; i < cursor; i++)
	{
		paths.push_back(crossover(paths[i-1],paths[i]));
		paths.push_back(crossover(paths[i], paths[i-1]));
	}
	paths.push_back(crossover(paths[0], paths[cursor - 1]));
	paths.push_back(crossover(paths[cursor - 1], paths[0]));
	// added 250*2 = 500 children

	//mutation
	for (int i = 0; i < 250; i++)
	{
		int random = rand() % 750;
		paths.push_back(mutate(paths[random]));
	}
}

void TSP()
{
	//generate start population 1000 paths
	for (int i = 0; i < 1000; i++)
	{
		random_shuffle(cities.begin(), cities.end());
		paths.push_back(Path(cities));
	}

	for (int i = 0; i < 500; i++)
	{
		sort(paths.begin(), paths.end());

		if ((i == 1) || (i == 10) || (i == 20) || (i == 50))
		{
			paths[0].print();
		}
		if (paths[0].fitness < 1592)
		{
			i = 500;
		}

		selection();
		addNewPaths();
	}
	sort(paths.begin(), paths.end());
	paths[0].print();
}


int main()
{
	srand((unsigned)time(0));

	cout << "N = ";
	cin >> N;

	//randomCoordinats();
	moodleExample();
	TSP();

	return 0;
}
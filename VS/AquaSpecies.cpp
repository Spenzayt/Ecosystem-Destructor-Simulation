#include <iostream>
#include "map_loader.h"
#include <cstdlib>
#include <chrono>
#include <time.h>
using namespace std;


class Bars :public Animals
{
public:
	float age = 0;
	int food;
	int maxFood = 100;
	int minFood = 60;
	int life = 100;
	bool alive = true;
	int elementNb;

	void isAlive() {
		cout << "te";
	}
	void move() {

	}

};
/*class Requin : public Animals
{
public:
	float age = 0;
	int food;
	int maxFood = 100;
	int minFood = 60;
	int life = 100;
	bool alive = true;
	int elementNb;

	Requin(int a, int e) : age(a), elementNb(e) {
		srand(time(NULL));
		int food = rand() % 100;

		cout << "la nouriture est initialis�e � " << food;
		return;
	}
	~Requin() {
		cout << elementNb << " est d�truit." << std::endl;
	}
	void getInfo() {
		cout << "L'�l�ment num�ro " << elementNb << "a pour age : " << age << ", sa barre de nourriture est � " << food << " et sa vie est � " << life << endl;
	}


};*/

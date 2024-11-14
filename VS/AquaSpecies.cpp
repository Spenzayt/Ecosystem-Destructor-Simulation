#include <iostream>
#include "animals.cpp"
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

		cout << "la nouriture est initialisée à " << food;
		return;
	}
	~Requin() {
		cout << elementNb << " est détruit." << std::endl;
	}
	void getInfo() {
		cout << "L'élément numéro " << elementNb << "a pour age : " << age << ", sa barre de nourriture est à " << food << " et sa vie est à " << life << endl;
	}


};*/
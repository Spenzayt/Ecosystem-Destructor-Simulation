#include <iostream>
#include "animals.cpp"
#include "map_loader.h"
#include <cstdlib>
#include <chrono>
#include <time.h>
using namespace std;
class Gobie : public Animals
{
public:


};
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








class Requin : public Animals
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
		srand(time(0));
		int food = rand() % 100;

		cout << "la nouriture est initialisée à " << food <<endl;
		return;
	}
	~Requin() {
		cout << elementNb << " est détruit." << std::endl;
	}
	void move() {

	}
	void spawn(Map& map) {
		bool finish = false;
		while (!finish) {
			Tile(*gameMap)[MapSize] = map.getMap();
			srand(time(0));
			int randx = rand() % 30;
			int randy = rand() % 30;

			if (gameMap[randx][randy].biome == WATER) {
				coords.x = randx;
				coords.y = randy;
				cout << coords.x << coords.y<<"est placé dans l'eau";
				finish = true;
			}
		}

		

	}

	void getInfo() {
		cout << "L'élément numéro " << elementNb << "a pour age : " << age << ", sa barre de nourriture est à " << food << " et sa vie est à " << life << endl;
	}


};
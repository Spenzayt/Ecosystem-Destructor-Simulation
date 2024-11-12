#include "display_functions.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>

using namespace std;


const int MapSize = 30;
const int MinResource = 20;
const int MaxResource = 40;


enum Biome {
	WATER,
	GRASS,
	RICH_GRASS
};


struct Tile {
	Biome biome;
	float resource;
};


class Map {
public:
	Tile map[MapSize][MapSize];
	bool lakeGeneration = false;

	void defaultMap() {
		for (int i = 0; i < MapSize; ++i) {
			for (int j = 0; j < MapSize; ++j) {
				map[i][j].biome = GRASS;
				map[i][j].resource = rand() % (MaxResource - MinResource + 1) + MinResource;
			}
		}
	}

	void displayMap() {
		for (int i = 0; i < MapSize; ++i) {
			cout << endl;
			for (int j = 0; j < MapSize; ++j) {
				setColorForBiome(map[i][j].biome);
				cout << "\xDB\xDB";
                setColor(7);
			}
		}
	}

	void setColorForBiome(Biome biome) {
		switch (biome) {
		case WATER:
			setColor(3);  // Blue
			break;
		case GRASS:
			setColor(2);  // Green
			break;
		case RICH_GRASS:
			setColor(10); // Light Green
			break;
		}
	}

    void generateWater() {
        int numLakes = rand() % 10 + 5;

        if (numLakes < 15) {
            generateLakes(numLakes);
        }

        if (numLakes == 15 || lakeGeneration) {
            generateLargeWaterBodies();
        }
    }

    void generateLakes(int numLakes) {
        for (int i = 0; i < numLakes; ++i) {
            int x = rand() % MapSize;
            int y = rand() % MapSize;
            map[x][y].biome = WATER;
            generateLakeRadius(x, y, rand() % 3 + 2);
        }
        lakeGeneration = true;
    }

    void generateLakeRadius(int x, int y, int radius) {
        for (int i = -radius; i <= radius; ++i) {
            for (int j = -radius; j <= radius; ++j) {
                int newX = x + i, newY = y + j;
                if (newX >= 0 && newX < MapSize && newY >= 0 && newY < MapSize) {
                    if ((i * i) + (j * j) <= (radius * radius)) {
                        map[newX][newY].biome = WATER;
                    }
                }
            }
        }
    }

    void generateLargeWaterBodies() {
        int numBodies = rand() % 40 + 20;
        for (int i = 0; i < numBodies; ++i) {
            int x = rand() % MapSize;
            int y = rand() % MapSize;
            if (map[x][y].biome != WATER) {
                map[x][y].biome = WATER;
            }
            else {
                --i;
            }
        }
    }

    void generateResources() {
        int numResources = rand() % 100 + 100;
        for (int i = 0; i < numResources; ++i) {
            int x = rand() % MapSize;
            int y = rand() % MapSize;
            if (map[x][y].biome == GRASS) {
                map[x][y].biome = RICH_GRASS;
                map[x][y].resource = 100;
            }
            else {
                --i;
            }
        }
    }

    void startGeneration() {
        defaultMap();
        generateWater();
        generateResources();
        displayMap();
    }

    void checkTile() {
        int x, y;
        cout << "Enter coordinates to check a Tile (x y): ";
        cin >> x >> y;
        if (x < 0 || x >= MapSize || y < 0 || y >= MapSize) {
            cout << "Invalid coordinates!" << endl;
            return;
        }

        clearScreen();
        cout << "Tile [" << x << "][" << y << "] Info:" << endl;
        switch (map[x][y].biome) {
        case WATER:
            cout << "Biome: Water" << endl;
            break;
        case GRASS:
            cout << "Biome: Grass" << endl;
            break;
        case RICH_GRASS:
            cout << "Biome: Rich Grass" << endl;
            break;
        }
        cout << "Remaining resources: " << map[x][y].resource << endl;
        displayMap();
    }

    void nextDay() {
        for (int i = 0; i < MapSize; ++i) {
            for (int j = 0; j < MapSize; ++j) {
                map[i][j].resource += 5;
            }
        }
        clearScreen();
        displayMap();
    }
};

Map map;

bool gameOver = false;

void displayMenu() {
    int choice;
    cout << endl;
    cout << "1 - Pass a day" << endl;
    cout << "2 - Check a tile" << endl;
    cout << "3 - Quit" << endl;
    cin >> choice;

    switch (choice) {
    case 1:
        map.nextDay();
        break;
    case 2:
        map.checkTile();
        break;
    case 3:
        gameOver = true;
        break;
    }
}


void startGame() {
    clearScreen();
    srand((unsigned)time(NULL));
    map.startGeneration();

    while (!gameOver) {
        displayMenu();
    }
}
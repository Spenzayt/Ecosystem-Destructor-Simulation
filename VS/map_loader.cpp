#include "display_functions.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>

using namespace std;

int const MapSize = 30;

struct Tiles
{
	int Biomes; // Pour le moment si une case est = 0 c'est de l'herbe et si c'est = 1 c'est de l'eau.
	int Ressource;
	int nbAnimalIn;
};

void setColor(int textColor)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int color = textColor;
	SetConsoleTextAttribute(hConsole, color);
}

class Map
{
public:
	Tiles Map[30][30];
	bool lacGeneration = false;

	int pw;
	int dayIndex = 1;

	bool Volcano;
	int PropaIndex;

	bool Flood;
	int FloodIndex;
	bool FloodStarted = false;

	bool Earthquake;

	void DisplayDay()
	{
		cout << "======================" << endl;
		cout << endl;
		cout << "       Jour : " << dayIndex << "    " << endl;
		cout << endl;
		cout << "======================" << endl;
	}

	void DisplayMap()
	{
		DisplayDay();
		for (int i = 0; i < MapSize; i++)
		{
			cout << endl;
			for (int j = 0; j < MapSize; j++)
				// 0 noir, 1 bleu fonc�, 2 vert, 3 turquoise, 4 rouge, 5 magenta, 6 jaune / marron, 7 gris clair
				// , 8 gris fonc�, 9 bleu clair, 10 vert clair, 11 cyan clair, 12 rouge clair, 13 rose, 14, jaune clair, 15 blanc
			{
				switch (Map[i][j].Biomes)
				{
				case 0:
					setColor(2);
					cout << "\xDB""\xDB";
					setColor(7);
					break;
				case 1:
					setColor(3);
					cout << "\xDB""\xDB";
					setColor(7);
					break;
				case 2:
					setColor(10);
					cout << "\xDB""\xDB";
					setColor(7);
					break;
				case 5:
					setColor(4);
					cout << "\xDB""\xDB";
					setColor(7);
					break;
				case 6:
					setColor(8);
					cout << "\xDB""\xDB";
					setColor(7);
					break;
				}
			}
		}
	}

	void InitialiseMap()
	{
		for (int i = 0; i < MapSize; i++)
		{
			for (int j = 0; j < MapSize; j++)
			{
				Map[i][j].Biomes = 0;
				Map[i][j].Ressource = (rand() % 40) + 20;
			}
		}
	}

	void GenerateWater()
	{
		int nbGeneration = (rand() % 10) + 5;

		if (nbGeneration < 15)
		{
			for (int i = 0; i < nbGeneration; i++) // Boucle de g�n�ration des lacs
			{

				int x = (rand() % MapSize);
				int y = (rand() % MapSize);

				Map[x][y].Biomes = 1;

				int radius = (rand() % 3) + 2;

				for (int j = -radius; j <= radius; j++)
				{
					for (int l = -radius; l <= radius; l++)
					{
						int newX = x + j;
						int newY = y + l;

						if (newX >= 0 && newX < MapSize && newY >= 0 && newY < MapSize)
						{
							if ((j * j) + (l * l) <= (radius * radius))
							{
								Map[newX][newY].Biomes = 1;
							}
						}
					}
				}
			}
			lacGeneration = true;
		}

		if (nbGeneration == 15 or lacGeneration == true) // G�n�ration d'une map grandement aquatique
		{
			pw = 800;
			if (lacGeneration == true)
			{
				lacGeneration = false;
				pw = (rand() % 40) + 20;
			}
			for (int i = 0; i < pw; i++)
			{
				int x = (rand() % MapSize);
				int y = (rand() % MapSize);
				if (Map[x][y].Biomes == 1)
				{
					i--;
				}
				else
				{
					Map[x][y].Biomes = 1;
				}
			}
		}
	}

	void GenerateRessource()
	{
		int nbRessource = (rand() % 100) + 100;
		for (int i = 0; i < nbRessource; i++)
		{
			int x = (rand() % MapSize);
			int y = (rand() % MapSize);

			if (Map[x][y].Biomes == 0)
			{
				Map[x][y].Biomes = 2;
				Map[x][y].Ressource = 100;
			}
			else
			{
				nbRessource--;
			}
		}
	}

	void GenerateVolcano()
	{
		int x = (rand() % MapSize);
		int y = (rand() % MapSize);

		Map[x][y].Biomes = 5;
		Map[x][y].Ressource = 0;

		for (int dx = -1; dx <= 1; dx++) {
			for (int dy = -1; dy <= 1; dy++) {
				if (dx == 0 && dy == 0)
					continue;

				int newX = x + dx;
				int newY = y + dy;

				if (newX >= 0 && newX < MapSize && newY >= 0 && newY < MapSize) {
					Map[newX][newY].Biomes = 6;
					Map[newX][newY].Ressource = 0;
				}
			}
		}
	}

	void PropagLava()
	{
		int LavaPropa = (rand() % 10) + 5;
	}

	void Flooding() // Fonctionne pas pour le moment
	{
		int nbFlood = (rand() % 3) + 3;

		if(FloodIndex < nbFlood)
		{
			for(int i = 0; i < MapSize; i++)
			{
				for(int j = 0; j < MapSize; j++)
				{
					if(Map[i][j].Biomes == 1)
					{
						i--; 
						j--;
					}
					else
					{
						Map[i][j].Biomes = 1;
					}
				}
			}
		}
		else
		{
			Flood = false;
		}
	}

	void StartGeneration()
	{
		InitialiseMap();
		GenerateWater();
		GenerateRessource();
		GenerateVolcano();
		DisplayMap();
	}

	void CheckTiles()
	{
		int x;
		int y;

		cout << "Les coordonnees de la case que vous voulez verifier" << endl;

		cin >> x;
		cin >> y;

		system("cls");

		cout << endl << "============================================================" << endl;
		cout << "===" << " La case [" << x << "][" << y << "]" << " contient les informations suivantes : " << "===" << endl;
		switch (Map[x][y].Biomes)
		{
		case 0: cout << "===" << " Biome : Terre                                        " << "===" << endl;
			break;
		case 1: cout << "===" << " Biome : Eau                                          " << "===" << endl;
			break;
		case 2: cout << "===" << " Biome : Terre riche                                  " << "===" << endl;
			break;
		}
		cout << "===" << " Ressources restantes : " << Map[x][y].Ressource << "                           " << "===" << endl;
		cout << "============================================================" << endl;

		DisplayMap();
	}

	void NewDay()
	{
		dayIndex++;
		for (int i = 0; i < MapSize; i++)
		{
			for (int j = 0; j < MapSize; j++)
			{
				Map[i][j].Ressource += 5;
			}
		}

		if(Volcano)
		{
			PropagLava();
		}

		if(Flood)
		{
			if(!FloodStarted)
			{
				FloodIndex = 0;
				FloodStarted = true;
			}
			Flooding();
		}

		system("cls");
		DisplayMap();
	}
=======

const int MapSize = 30;
const int MinResource = 20;
const int MaxResource = 40;


enum Biome {
    WATER,
    GRASS,
    RICH_GRASS,
    ROCK,
    LAVA,
    FLOODING_WATER,
};


struct Tile {
    Biome biome;
    float resource;
};


class Map {
public:
    Tile map[MapSize][MapSize];
    bool lakeGeneration = false;

    // Flooding related variables.
    bool isFlooding = false;
    int nbFlood;
    int FloodIndex = 0;
    bool isUnFlooding = false;

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
        case ROCK:
            setColor(8); // Grey
            break;
        case LAVA:
            setColor(4); // Red
            break;
        case FLOODING_WATER:
            setColor(3);
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

    void generateVolcano() {
        int x = (rand() % MapSize);
        int y = (rand() % MapSize);
        map[x][y].biome = LAVA;
        map[x][y].resource = 0;
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0)
                    continue;
                int newX = x + dx;
                int newY = y + dy;
                if (newX >= 0 && newX < MapSize && newY >= 0 && newY < MapSize) {
                    map[newX][newY].biome = ROCK;
                    map[newX][newY].resource = 0;
                }
            }
        }
    }

    void StartFlooding() // Fonctionne pas pour le moment
    {
        nbFlood = (rand() % 3) + 2;
        FloodIndex = 0;
        isFlooding = true;
    }

    void StartUnFlooding() {
        isUnFlooding = true;
    }

    void ContinueFlooding() {
        int nbFloodedTiles = 40;
        if (FloodIndex < nbFlood) {
            for (int i = 0; i < nbFloodedTiles; i++) {
                int x = (rand() % MapSize);
                int y = (rand() % MapSize);
                if (map[x][y].biome != WATER && map[x][y].biome != ROCK && map[x][y].biome != LAVA && map[x][y].biome != FLOODING_WATER) {
                    map[x][y].biome = FLOODING_WATER;
                    map[x][y].resource /= 2;
                }
                else {
                    i--;
                }
            }
        }
        else {
            isFlooding = false;
        }
        FloodIndex++;
    }

    void startGeneration() {
        defaultMap();
        generateWater();
        generateResources();
        generateVolcano();
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
        case FLOODING_WATER:
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

        if (isFlooding) {
            ContinueFlooding();
        }
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
    cout << "Temporary Command : " << endl;
    cout << "4 - Flood map" << endl;
    cout << "5 - Wake volcano (not working)" << endl;
    cout << "6 - Earthquake (not working" << endl;
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
    case 4:
        map.StartFlooding();
        map.nextDay();
        break;


    default:
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
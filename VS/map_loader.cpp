#include<iostream>
#include<cstdlib>
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
				// 0 noir, 1 bleu foncé, 2 vert, 3 turquoise, 4 rouge, 5 magenta, 6 jaune / marron, 7 gris clair
				// , 8 gris foncé, 9 bleu clair, 10 vert clair, 11 cyan clair, 12 rouge clair, 13 rose, 14, jaune clair, 15 blanc
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
			for (int i = 0; i < nbGeneration; i++) // Boucle de génération des lacs
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

		if (nbGeneration == 15 or lacGeneration == true) // Génération d'une map grandement aquatique
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
};

Map map;

bool End = false;

void menuTest()
{
	int choice;
	cout << endl;
	cout << "1 - Passer un jour" << endl;
	cout << "2 - Check une case" << endl;
	cout << "3 - Declencher le volcan" << endl;
	cout << "4 - Declencher une inondation" << endl;
	cout << "5 - Delencher un seisme" << endl;
	cout << "6 - Quitter" << endl;
	cin >> choice;

	switch (choice)
	{
	case 1:
		map.NewDay();
		break;
	case 2:
		map.CheckTiles();
		break;
	case 3:
		map.Volcano = true;
		map.NewDay();
		break;
	case 4:
		map.Flood = true;
		map.NewDay();
		break;
	case 5:
		break;
	case 6: End = true; 
		break;
	}
}

void startMap()
{
	srand((unsigned)time(NULL));
	map.StartGeneration();

	while (!End)
	{
		menuTest();
	}
}
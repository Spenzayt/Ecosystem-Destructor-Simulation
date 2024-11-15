#ifndef MAP_LOADER_H
#define MAP_LOADER_H


#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <ctime>
#include <conio.h> // pour _getch()
#include <utility>
#include <thread>
#include <chrono>

using namespace std;

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
    VOLCANO_LAVA,
    CRATER,
    DAMAGED,
    BURNED,
};

struct Tile {
    Biome biome;
    int resource;
    bool AnimalIn;
};
struct Coordinates {
    float x;
    float y;
};

class Map {
public:
    int days = 1;
    Tile map[MapSize][MapSize];

    bool lakeGeneration;
    Tile(*getMap())[MapSize];

    bool isFlooding;
    int nbFlood;
    int FloodIndex;
    bool isUnFlooding;

    int volcanoX, volcanoY;
    int lavaRadius;
    int lavaDays;
    bool isErupting;

    void defaultMap();
    void displayMap();
    void setColorForBiome(Biome biome);

    void StartFlooding();
    void ContinueFlooding();
    void StartUnFlooding();
    void ContinueUnFlooding();
    void StartVolcano();
    void PropagateLava();
    void LavaToRock();
    void startGeneration();
    void checkTile();
    void nextDay();
    vector<pair<int, int>> getTiles(Biome biom);
};

extern Map map;
extern bool gameOver;

void displayMenu();
void startGame();
class Animals {
private:
    Coordinates coords;
    float orientation;
    float age;
    int maxFood;
    int currentFood;
    string specie;
    int dailyEat;
    float speed;
    Biome type;

public:
    Animals(Biome type, string specie, int maxFood, int dailyEat, float speed = 1.0f);

    Coordinates getCoords();
    void SetCoords(float x, float y);
    float GetDirection();
    void Move(Tile map[30][30]);
    void Eat(Tile map[30][30]);
};

// Classe SpecieManager
class SpecieManager {
public:
    std::vector<Animals> AnimalList;
    Tile map[30][30];
    Biome type;
    std::string specie;
    int maxFood;
    int dailyEat;
    float speed;

    SpecieManager(Tile(&mapp)[30][30], Biome type, std::string specie, int maxFood, int dailyEat, float speed = 1.0f);

    void AddAnimal();
    int GetNumberAnimal();
    vector<Animals> GetAnimalsArround(float x, float y, float radius = 1);
    void MoveAnimals(Tile map[30][30]);
};

// Variables globales
extern bool gameOver;
extern Map map;
extern SpecieManager GobieManager;
extern SpecieManager SharkManager;

enum Key {
    INVALID = -1, UP, DOWN, LEFT, RIGHT, SPACE = 32, ENTER = 13, Z = 'z', S = 's', A = 'a', B = 'b'
};
// Fonctions utilitaires
Key getKeyInput();
bool checkKonamiCode(const vector<Key>& inputs);
void displayGameMenuOptions(int choice, bool konamiCodeActivated);

// Fonctions principales du jeu
void nextDay();
void displayMenu();
void startGame();

#endif // GAME_H
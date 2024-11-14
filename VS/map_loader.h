#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include <iostream>
#include <vector>

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

#endif

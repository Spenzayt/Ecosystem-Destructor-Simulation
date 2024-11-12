#ifndef MAP_H
#define MAP_H

#include <iostream>

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

    bool lakeGeneration;

    bool isFlooding;
    int nbFlood;
    int FloodIndex;
    bool isUnFlooding;

    void defaultMap();
    void displayMap();
    void setColorForBiome(Biome biome);
    void generateWater();
    void generateLakes(int numLakes);
    void generateLakeRadius(int x, int y, int radius);
    void generateLargeWaterBodies();
    void generateResources();
    void generateVolcano();
    void StartFlooding();
    void StartUnFlooding();
    void ContinueFlooding();
    void startGeneration();
    void checkTile();
    void nextDay();
};

extern Map map;
extern bool gameOver;

void displayMenu();
void startGame();

#endif 

#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "display_functions.h"

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

    void defaultMap();
    void displayMap();
    void setColorForBiome(Biome biome);
    void generateWater();
    void generateLakes(int numLakes);
    void generateLakeRadius(int x, int y, int radius);
    void generateLargeWaterBodies();
    void generateResources();

    void startGeneration();
    void checkTile();
    void nextDay();
};

void startGame();


#endif

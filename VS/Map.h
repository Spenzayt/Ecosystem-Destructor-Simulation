#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <cstdlib>
#include <windows.h>

using namespace std;

const int MapSize = 30;

struct Tiles
{
    int Biomes;
    float Ressource;
};

class Map
{
public:
    Tiles Map[MapSize][MapSize];
    bool lacGeneration = false;
    int pw = 40;

    void AfficherMap();
    void InitialiseMap();
    void GenerateWater();
    void GenerateRessource();
    void StartGeneration();
    void CheckTiles();
    void NewDay();
};

extern Map map;

void menuTest();
void startMap();

#endif // MAP_H

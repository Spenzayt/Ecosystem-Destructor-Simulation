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
    int pw;
    int dayIndex = 1;

    bool Volcano;
    int PropaIndex;

    bool Flood;
    int FloodIndex;
    bool FloodStarted = false;

    bool Earthquake;

    void DisplayMap();
    void InitialiseMap();
    void GenerateWater();
    void GenerateVolcano();
    void GenerateRessource();
    void PropagLava();
    void Flooding();
    void StartGeneration();
    void CheckTiles();
    void NewDay();
};

extern Map map;

void menuTest();
void startMap();

#endif // MAP_H

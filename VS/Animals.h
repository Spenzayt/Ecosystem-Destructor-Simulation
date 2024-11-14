#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include "display_functions.h"

using namespace std;

// ---------------------------------------------------------------------------------
// Configuration de la carte et types de données
// ---------------------------------------------------------------------------------

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

// ---------------------------------------------------------------------------------
// Classe Map : gestion de la carte et génération des biomes
// ---------------------------------------------------------------------------------

class Map {
public:
    int days = 1;
    Tile map[MapSize][MapSize];
    bool lakeGeneration = false;

    Tile(*getMap())[MapSize] {
        return map;
        }

        // Variables pour les inondations
    bool isFlooding = false;
    int nbFlood;
    int FloodIndex = 0;
    bool isUnFlooding = false;

    // Variables pour les volcans
    int volcanoX, volcanoY;
    int lavaRadius = 1;
    int lavaDays = 0;
    bool isErupting = false;

    // Méthodes de génération et affichage
    void defaultMap();
    void displayMap();
    void setColorForBiome(Biome biome);
    void generateWater();
    void generateLakes(int numLakes);
    void generateLakeRadius(int x, int y, int radius);
    void generateLargeWaterBodies();
    void generateResources();
    void generateVolcano();

    // Méthodes pour les événements naturels
    void StartFlooding();
    void StartUnFlooding();
    void ContinueFlooding();
    void ContinueUnFlooding();
    void StartVolcano();
    void PropagateLava();
    void LavaToRock();

    // Autres méthodes de gestion de la carte
    void SpawnTest();
    void startGeneration();
    void checkTile();
    void nextDay();
    void nuke(int x, int y, int radius);

    // Autres méthodes
    vector<pair<int, int>> getTiles(Biome biom);
};

// ---------------------------------------------------------------------------------
// Gestion des animaux : struct Coordinates et classe Animals
// ---------------------------------------------------------------------------------

struct Coordinates {
    float x;
    float y;
};

class Animals {
private:
    Coordinates coords;
    float orientation = 0;
    float age = 0;
    int maxFood;
    int currentFood;
    string specie;
    int dailyEat;
    float speed;
    Biome type;

public:
    Animals(Map& map);
    Coordinates getCoords();
    void SetCoords(float x, float y);
    float GetDirection();
    void Move(Map& map);
    void Eat(Map& map);
};

// ---------------------------------------------------------------------------------
// Gestion des espèces : classe SpecieManager
// ---------------------------------------------------------------------------------

class SpecieManager {
public:
    std::vector<Animals> AnimalList;
    string name;

    SpecieManager(string n) : name(n) {}

    void AddAnimal(Map& map);
    int GetNumberAnimal();
    std::vector<Animals> GetAnimalsArround(float x, float y, float radius = 1);
    void ShowCoords();
    void MoveAnimals(Map& map);
};

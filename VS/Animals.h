#ifndef ANIMALS_H
#define ANIMALS_H

#include <iostream>
#include <vector>
#include <string>
#include "display_functions.h"

// Taille de la carte et ressources
const int MapSize = 30;
const int MinResource = 20;
const int MaxResource = 40;

// Définition des types de biomes
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

// Structure de tuiles de la carte
struct Tile {
    Biome biome;
    int resource;
    bool AnimalIn;
};

// Classe de gestion de la carte
class Map {
public:
    int days;
    Tile map[MapSize][MapSize];
    bool lakeGeneration;

    // Variables spécifiques aux espèces et aux phénomènes naturels

    bool isFlooding;
    int nbFlood;
    int FloodIndex;
    bool isUnFlooding;
    int volcanoX, volcanoY;
    int lavaRadius;
    int lavaDays;
    bool isErupting;

    Map();
    Tile(*getMap())[MapSize];
    std::vector<std::pair<int, int>> getTiles(Biome biom);
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
    void ContinueUnFlooding();
    void StartVolcano();
    void PropagateLava();
    void LavaToRock();
    void SpawnTest();
    void startGeneration();
    void checkTile();
    void nextDay();
    void nuke(int x, int y, int radius);
};

// Structure de coordonnées
struct Coordinates {
    float x;
    float y;
};

// Classe Animaux
class Animals {
private:
    Coordinates coords;
    float orientation;
    float age;
    int maxFood;
    int currentFood;
    std::string specie;
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

// Classe de gestion des espèces
class SpecieManager {
public:
    std::vector<Animals> AnimalList;
    std::string name;

    SpecieManager(std::string n);
    void AddAnimal(Map& map);
    int GetNumberAnimal();
    std::vector<Animals> GetAnimalsArround(float x, float y, float radius = 1);
    void ShowCoords();
    void MoveAnimals(Map& map);
};

#endif // ANIMALS_H

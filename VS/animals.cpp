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

// MAP

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
    bool lakeGeneration = false;

    Tile(*getMap())[MapSize] {
        return map;
        }

    // Flooding related variables.
    bool isFlooding = false;
    int nbFlood;
    int FloodIndex = 0;
    bool isUnFlooding = false;

    //Volcano related variables.
    int volcanoX, volcanoY;
    int lavaRadius = 1;
    int lavaDays = 0;
    bool isErupting = false;

    vector<pair<int, int>> getTiles(Biome biom) {
        vector<pair<int, int>> Tiles;
        for (int i = 0; i < MapSize; i++)
        {
            for (int j = 0; j < MapSize; j++)
            {
                if (map[i][j].biome == biom)
                {
                    Tiles.push_back({ i, j });
                }
            }
        }
    }

    void defaultMap() {
        for (int i = 0; i < MapSize; ++i) {
            for (int j = 0; j < MapSize; ++j) {
                map[i][j].biome = GRASS;
                map[i][j].resource = rand() % (MaxResource - MinResource + 1) + MinResource;
                map[i][j].AnimalIn = false;
            }
        }
    }

    void displayMap() {
        setColorBg(0);
        cout << endl;
        for (int i = 0; i < MapSize; ++i) {
            centerText(" ", false, (MapSize * 2));
            for (int j = 0; j < MapSize; ++j) {
                setColorForBiome(map[i][j].biome);
                if (map[i][j].AnimalIn) {
                    setColorText(0);
                    cout << "0 ";
                }
                else {
                    cout << "  ";
                }
            }
            cout << endl;
            setColorBg(0);
        }
        setColorText(7);
    }

    void setColorForBiome(Biome biome) {
        int bgColor;

        switch (biome) {
        case WATER:
            bgColor = 3; // Blue
            break;
        case GRASS:
            bgColor = 2;  //Green
            break;
        case RICH_GRASS:
            bgColor = 10; // Light Green
            break;
        case ROCK:
            bgColor = 8;  // Grey
            break;
        case LAVA:
            bgColor = 4;  // Red
            break;
        case FLOODING_WATER:
            bgColor = 3;
            break;
        case VOLCANO_LAVA:
            bgColor = 4;
            break;
        case CRATER:
            bgColor = 4;
            break;
        case DAMAGED:
            bgColor = 12;
            break;
        case BURNED:
            bgColor = 6;
            break;
        default:
            bgColor = 0;
            break;
        }
        setColorBg(bgColor);
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
        int numResources = rand() % 100 + 50;
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
        volcanoX = (rand() % MapSize);
        volcanoY = (rand() % MapSize);
        map[volcanoX][volcanoY].biome = VOLCANO_LAVA;
        map[volcanoX][volcanoY].resource = 0;
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0)
                    continue;
                int newX = volcanoX + dx;
                int newY = volcanoY + dy;
                if (newX >= 0 && newX < MapSize && newY >= 0 && newY < MapSize) {
                    map[newX][newY].biome = ROCK;
                    map[newX][newY].resource = 0;
                }
            }
        }
    }

    void StartFlooding()
    {
        nbFlood = (rand() % 3) + 2;
        FloodIndex = 0;
        isFlooding = true;
    }

    void StartUnFlooding() {
        isUnFlooding = true;
        FloodIndex = 0;
    }

    void ContinueFlooding() {
        int nbFloodedTiles = 40;
        if (FloodIndex < nbFlood) {
            for (int i = 0; i < nbFloodedTiles; i++) {
                int x = (rand() % MapSize);
                int y = (rand() % MapSize);
                if (map[x][y].biome != WATER && map[x][y].biome != ROCK && map[x][y].biome != LAVA && map[x][y].biome != FLOODING_WATER && map[x][y].biome != VOLCANO_LAVA) {
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
            FloodIndex = 0;
            StartUnFlooding();
        }
        FloodIndex++;
    }

    void ContinueUnFlooding() {
        FloodIndex++;
        if (FloodIndex > 4) {
            for (int i = 0; i < MapSize; i++) {
                for (int j = 0; j < MapSize; j++) {
                    if (map[i][j].biome == FLOODING_WATER) {
                        if (rand() % 4 == 1) {
                            map[i][j].biome = GRASS;
                        }
                    }
                }
            }
        }
        else if (FloodIndex > 10) {
            for (int i = 0; i < MapSize; i++) {
                for (int j = 0; j < MapSize; j++) {
                    if (map[i][j].biome == FLOODING_WATER) {
                        map[i][j].biome = GRASS;
                        FloodIndex = 0;
                        isUnFlooding = false;
                    }
                }
            }
        }
    }

    void StartVolcano() {
        isErupting = true;
        lavaRadius = 1;
        lavaDays = 0;
    }

    void PropagateLava() {
        if (!isErupting) return;
        lavaDays++;
        for (int i = -lavaRadius; i <= lavaRadius; ++i) {
            for (int j = -lavaRadius; j <= lavaRadius; ++j) {
                int newX = volcanoX + i;
                int newY = volcanoY + j;
                if (newX >= 0 && newX < MapSize && newY >= 0 && newY < MapSize) {
                    if (map[newX][newY].biome != ROCK && map[newX][newY].biome != LAVA && map[newX][newY].biome != VOLCANO_LAVA) {
                        map[newX][newY].biome = LAVA;
                        map[newX][newY].resource = 0;
                        map[newX][newY].AnimalIn = false;
                    }
                }
            }
        }
        lavaRadius++;
        if (lavaRadius > 20) {
            lavaRadius = 20;
        }
    }

    void LavaToRock() {
        if (!isErupting || lavaDays < 4) return;

        for (int i = -lavaRadius; i <= lavaRadius; ++i) {
            for (int j = -lavaRadius; j <= lavaRadius; ++j) {
                int newX = volcanoX + i;
                int newY = volcanoY + j;
                if (newX >= 0 && newX < MapSize && newY >= 0 && newY < MapSize) {
                    if (map[newX][newY].biome == LAVA) {
                        map[newX][newY].biome = ROCK;
                    }
                }
            }
        }
        isErupting = false;
        lavaRadius = 1;
        lavaDays = 0;
    }

    void SpawnTest()
    {
        int x = (rand() % MapSize);
        int y = (rand() % MapSize);

        map[x][y].AnimalIn = true;
    }

    void startGeneration() {
        defaultMap();
        generateWater();
        generateResources();
        generateVolcano();
        SpawnTest();
        displayMap();
    }

    void checkTile() {
        int x, y;
        cout << "Enter coordinates to check a Tile (x y): " << endl;
        cout << "X -> ";
        cin >> x;
        cout << "Y -> ";
        cin >> y;
        x--;
        y--;
        if (x < 0 || x >= MapSize || y < 0 || y >= MapSize) {
            cout << "Invalid coordinates!" << endl;
            return;
        }

        clearScreen();
        displayLittleTitle();
        displayMap();
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
        case LAVA:
            cout << "Biome: Lava" << endl;
            break;
        case ROCK:
            cout << "Biome: Rock" << endl;
            break;
        case VOLCANO_LAVA:
            cout << "Biome: Lava" << endl;
            break;
        }
        cout << "Remaining resources: " << map[x][y].resource << endl;
    }

    void nextDay() {
        int daysToPass = 1;

        while (true) {
            clearScreen();
            displayLittleTitle();
            displayMap();
            cout << "How many days do you want to pass ?" << endl;
            cout << "Day to pass -> " << daysToPass;

            char input = _getch();

            if (input == 'z' || input == 'Z') {
                daysToPass++;
            }
            else if (input == 's' || input == 'S') {
                daysToPass--;
            }
            else if (input == 72) {
                daysToPass++;
            }
            else if (input == 80) {
                daysToPass--;
            }
            else if (input == 13) {
                break;
            }
        }

        for (int day = 0; day < daysToPass; day++) {
            int x = (rand() % 99) + 1;
            if (x <= 80) {
            }
            if (x > 80 && x < 90) {
                StartFlooding();
            }
            if (x > 90) {
                StartVolcano();
            }
            if (isFlooding) {
                ContinueFlooding();
            }
            if (isUnFlooding) {
                ContinueUnFlooding();
            }
            if (isErupting) {
                PropagateLava();
                LavaToRock();
            }
            for (int i = 0; i < MapSize; ++i) {
                for (int j = 0; j < MapSize; ++j) {
                    map[i][j].resource += rand() % 3;
                }
            }

            // Animal Update
            // 
            //GobieManager.moveAnimals();


            days++;
            clearScreen();
            displayLittleTitle();
            displayMap();
            cout << endl << endl << endl;
            centerText(to_string(day) + " days pass", true, 0);
            this_thread::sleep_for(chrono::seconds(1));
        }
    }
    void nuke(int x, int y, int radius) {
        for (int dx = -radius; dx <= radius; ++dx) {
            for (int dy = -radius; dy <= radius; ++dy) {
                int newX = x + dx;
                int newY = y + dy;
                if (newX >= 0 && newX < MapSize && newY >= 0 && newY < MapSize) {
                    int distance = std::sqrt(dx * dx + dy * dy);
                    if (distance <= radius) {
                        if (distance <= radius / 3) {
                            map[newX][newY].biome = CRATER;
                        }
                        else if (distance <= 2 * radius / 3) {
                            map[newX][newY].biome = DAMAGED;
                        }
                        else {
                            map[newX][newY].biome = BURNED;
                        }
                        map[newX][newY].resource = 0;
                    }
                }
            }
        }
    }

};

// ANIMALS

struct Coordinates
{
    float x;
    float y;
};

class Animals {
private:
    Coordinates coords;
    float orientation = 0;
    float age = 0;
    int maxFood; // random
    int currentFood;
    string specie; //TODO : definir ca pour chaque esp�ce
    int dailyEat;
    float speed;
    Biome type;

public:

    Animals(Map& map)
    {;
        vector<pair<int, int>> goodTiles = map.getTiles(type);
        pair<int, int> tile = goodTiles[int(rand() % goodTiles.size())];
        coords.x = tile.first;
        coords.y = tile.second;

    }

    Coordinates getCoords()
    {
        return coords;
    }
    void SetCoords(float x, float y)
    {
        coords.x = x;
        coords.y = y;
    }

    float GetDirection()
    {
        return orientation;
    }

    void Move(Map& map)
    {
        Tile(*gameMap)[MapSize] = map.getMap();

        currentFood += -1;

        if (currentFood < 0.3f * maxFood) // Si il a moins de 33% de nourriture
        {
            if (gameMap[int(coords.x)][int(coords.y)].resource > dailyEat) // Si y'a a manger
            {
                Eat(map);
            }
            else
            {
                int dx = round(cos(orientation));
                int dy = round(sin(orientation));

                if (gameMap[int(coords.x) + dx][int(coords.y) + dy].resource > dailyEat) // Si la case devant lui a a manger, avancer
                {
                    coords.x += cos(orientation) * speed;
                    coords.y += sin(orientation) * speed;
                }

                else
                {
                    orientation += 1; // environ 60 degr�s de plus
                }
            }
        }
        else
        {
            float randMovement = (float((rand() % 9) + 1) / 10);
            coords.x += cos(orientation) * speed * randMovement;
            coords.y += sin(orientation) * speed * randMovement;

            orientation -= (float((rand() % 9) + 1) / 10);
        }
    }

    void Eat(Map& map)
    {
        Tile(*gameMap)[MapSize] = map.getMap();
        
        currentFood += dailyEat;
        gameMap[int(coords.x)][int(coords.y)].resource - dailyEat;

    }
};

class SpecieManager
{
public:
    std::vector<Animals> AnimalList;
    string name;

    SpecieManager(string n) : name(n)
    {
    }

    void AddAnimal(Map& map)
    {
        AnimalList.push_back(Animals(map));
    }


    int GetNumberAnimal()
    {
        int counter = 0;
        for (auto animal : AnimalList)
        {
            counter += 1;
        }
        return counter;
    }

    std::vector<Animals> GetAnimalsArround(float x, float y, float radius = 1)
    {
        //return the number of animal of that specie arround a place, with a radius (by default radius = 1).
        // Dans une zone carr�e
        std::vector<Animals> closeList;
        for (auto animal : AnimalList)
        {
            Coordinates animalCoords = animal.getCoords();
            if ((animalCoords.x > x - radius && animalCoords.x < x + radius) &&
                (animalCoords.y > y - radius && animalCoords.y < y + radius))
            {
                closeList.push_back(animal);
            }
        }
        return closeList;
    }

    void ShowCoords()
    {
        int counter = 0;
        for (auto animal : AnimalList)
        {
            counter += 1;
            Coordinates coords = animal.getCoords();
            cout << name << counter << ": " << coords.x << ", " << coords.y << endl;
        }
    }

    void MoveAnimals(Map& map)
    {
        for (auto animal : AnimalList)
        {
            animal.Move(map);
        }
    }
};

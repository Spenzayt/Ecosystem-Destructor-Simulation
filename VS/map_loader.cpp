#include "display_functions.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>

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
};


struct Tile {
    Biome biome;
    int resource;
    bool AnimalIn;
};


class Map {
public:
    Tile map[MapSize][MapSize];
    bool lakeGeneration = false;

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
        for (int i = 0; i < MapSize; ++i) {
            cout << endl;
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
        int numResources = rand() % 100 + 100;
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

    void StartFlooding() // Fonctionne pas pour le moment
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
                if (map[x][y].biome != WATER && map[x][y].biome != ROCK && map[x][y].biome != LAVA && map[x][y].biome != FLOODING_WATER) {
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

    void StartEruption() {
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
                    }
                }
            }
        }
        lavaRadius++;
        if (lavaRadius > 15) {
            lavaRadius = 15;
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
        cout << "Enter coordinates to check a Tile (x y): ";
        cin >> x >> y;
        if (x < 0 || x >= MapSize || y < 0 || y >= MapSize) {
            cout << "Invalid coordinates!" << endl;
            return;
        }

        clearScreen();
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
        }
        cout << "Remaining resources: " << map[x][y].resource << endl;
        displayMap();
    }

    void nextDay() {

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
                map[i][j].resource += 5;
            }
        }
        clearScreen();
        displayMap();
        cout << endl << FloodIndex << endl;
    }
};

Map map;

bool gameOver = false;

void displayMenu() {
    int choice;
    cout << endl;
    cout << "1 - Pass a day" << endl;
    cout << "2 - Check a tile" << endl;
    cout << "3 - Quit" << endl;
    cout << "Temporary Command : " << endl;
    cout << "4 - Flood map" << endl;
    cout << "5 - Wake volcano (not working)" << endl;
    cout << "6 - Earthquake (not working" << endl;
    cin >> choice;

    switch (choice) {
    case 1:
        map.nextDay();
        break;
    case 2:
        map.checkTile();
        break;
    case 3:
        gameOver = true;
        break;
    case 4:
        map.StartFlooding();
        map.nextDay();
        break;
    case 5:
        map.StartEruption();
        map.nextDay();
        break;
    default:
        break;
    }
}


void startGame() {
    clearScreen();
    srand((unsigned)time(NULL));
    map.startGeneration();

    while (!gameOver) {
        displayMenu();
    }
}
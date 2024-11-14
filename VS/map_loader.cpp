#include "display_functions.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include <vector>
#include <string>
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
};


struct Tile {
    Biome biome;
    float resource;
};


class Map {
public:
    int days = 1;
    Tile map[MapSize][MapSize];
    bool lakeGeneration = false;

    // Flooding related variables.
    bool isFlooding = false;
    int nbFlood;
    int FloodIndex = 0;
    bool isUnFlooding = false;

    void defaultMap() {
        for (int i = 0; i < MapSize; ++i) {
            for (int j = 0; j < MapSize; ++j) {
                map[i][j].biome = GRASS;
                map[i][j].resource = rand() % (MaxResource - MinResource + 1) + MinResource;
            }
        }
    }

    void displayMap() {
        cout << endl;
        for (int i = 0; i < MapSize; ++i) {
            setColorForBiome(map[i][0].biome);
            centerText("\xDB\xDB", false, MapSize * 2);
            for (int j = 1; j < MapSize; ++j) {
                setColorForBiome(map[i][j].biome);
                cout << "\xDB\xDB";
                setColor(7);
            }
            cout << endl;
        }
    }

    void setColorForBiome(Biome biome) {
        switch (biome) {
        case WATER:
            setColor(3);  // Blue
            break;
        case GRASS:
            setColor(2);  // Green
            break;
        case RICH_GRASS:
            setColor(10); // Light Green
            break;
        case ROCK:
            setColor(8); // Grey
            break;
        case LAVA:
            setColor(4); // Red
            break;
        case FLOODING_WATER:
            setColor(3);
            break;
        }
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
        int x = (rand() % MapSize);
        int y = (rand() % MapSize);
        map[x][y].biome = LAVA;
        map[x][y].resource = 0;
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0)
                    continue;
                int newX = x + dx;
                int newY = y + dy;
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
        }
        FloodIndex++;
    }

    void startGeneration() {
        defaultMap();
        generateWater();
        generateResources();
        generateVolcano();
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
            if (isFlooding) {
                ContinueFlooding();
            }
            for (int i = 0; i < MapSize; ++i) {
                for (int j = 0; j < MapSize; ++j) {
                    map[i][j].resource += 5;
                }
            }
            days++;
            clearScreen();
            displayLittleTitle();
            displayMap();
            cout << endl << endl << endl;
            centerText(to_string(day) + " days pass", true, 0);
            this_thread::sleep_for(chrono::seconds(2));
        }
    }
};

Map map;

bool gameOver = false;

enum Key {
    INVALID = -1, UP, DOWN, LEFT, RIGHT, SPACE = 32, ENTER = 13, Z = 'z', S = 's', A = 'a', B = 'b'
};

Key getKeyInput() {
    int ch = _getch();

    if (ch == 0 || ch == 224) {
        ch = _getch();
        switch (ch) {
        case 72: return UP;
        case 80: return DOWN;
        case 77: return RIGHT;
        case 75: return LEFT;
        }
    }

    if (ch == ' ') return SPACE;
    if (ch == ENTER) return ENTER;
    if (ch == 'z' || ch == 'Z') return Z;
    if (ch == 's' || ch == 'S') return S;
    if (ch == 'b' || ch == 'B') return B;
    if (ch == 'a' || ch == 'A') return A;

    return INVALID;
}

bool checkKonamiCode(const vector<Key>& inputs) {
    const vector<Key> konamiCode = { UP, UP, DOWN, DOWN, LEFT, RIGHT, LEFT, RIGHT, B, A };
    if (inputs.size() < konamiCode.size()) {
        return false;
    }

    for (size_t i = 0; i < konamiCode.size(); ++i) {
        if (inputs[inputs.size() - konamiCode.size() + i] != konamiCode[i]) {
            return false;
        }
    }
    return true;
}


void displayGameMenuOptions(const int choice, bool konamiCodeActivated) {
    clearScreen();
    displayLittleTitle();
    map.displayMap();

    cout << endl << endl;
    centerText("Day " + to_string(map.days), true, 0);
    centerText("========================================", true, 0);
    centerText((choice == 1 ? ">  1 - Pass a day" : "   1 - Pass a day"), true, 0);
    centerText((choice == 2 ? ">  2 - Check a tile" : "   2 - Check a tile"), true, 0);
    centerText((choice == 3 ? ">  3 - Quit" : "   3 - Quit"), true, 0);


    if (konamiCodeActivated) {
        cout << endl;
        centerText("       Cheat Command : ", true, 0);
        centerText((choice == 4 ? ">  4 - Flood map" : "   4 - Flood map"), true, 0);
        centerText((choice == 5 ? ">  5 - Wake volcano (not working)" : "   5 - Wake volcano (not working)"), true, 0);
        centerText((choice == 6 ? ">  6 - Earthquake (not working)" : "   6 - Earthquake (not working)"), true, 0);
        centerText((choice == 7 ? ">  7 - Nuke (not working)" : "   7 - Nuke (not working)"), true, 0);
    }
    centerText("========================================", true, 0);
}


void displayMenu() {
    int choice = 1;
    bool konamiCodeActivated = false;
    vector<Key> inputs;

    while (true) {
        displayGameMenuOptions(choice, konamiCodeActivated);
        Key input = getKeyInput();

        if (input != INVALID) {
            inputs.push_back(input);

            if (checkKonamiCode(inputs)) {
                konamiCodeActivated = true;
                inputs.clear();
            }
        }

        if (input == UP || input == Z){
            if (choice > 1) {
                choice--;
            }
        }
        else if (input == DOWN || input == S) {
            if (choice < (konamiCodeActivated ? 7 : 3)) {
                choice++;
            }
        }
        else if (input == SPACE || input == ENTER) {
            if (choice == 1) {
                map.nextDay();
            }
            else if (choice == 2) {
                map.checkTile();
            }
            else if (choice == 3) {
                gameOver = true;
                return;
            }
            else if (choice == 4) {
                map.StartFlooding();
                map.nextDay();
            }
            else if (choice == 5) {
                // Wake Volcano
            }
            else if (choice == 6) {
                // Earthquake
            }
            else if (choice == 7) {
                // Nuke
            }
        }
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
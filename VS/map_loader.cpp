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
            if (x <= 80){
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
                    map[i][j].resource += 5;
                }
            }
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

Map map;

bool gameOver = false;

enum Key {
    INVALID = -1, UP, DOWN, LEFT, RIGHT, SPACE = 32, ENTER = 13, Z = 'z', S = 's', A = 'a', B = 'b'
};

Key getKeyInput() {
    int ch = _getch();
    clearScreen();
    displayLittleTitle();
    map.displayMap();

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
        centerText((choice == 5 ? ">  5 - Wake volcano" : "   5 - Wake volcano"), true, 0);
        centerText((choice == 6 ? ">  6 - Nuke" : "   6 - Nuke"), true, 0);
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
                map.generateVolcano();
                map.nextDay();
            }
            else if (choice == 6) {
                map.nuke(15,15,16);
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

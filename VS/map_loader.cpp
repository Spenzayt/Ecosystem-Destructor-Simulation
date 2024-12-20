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

// NEW

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

enum AnimalsList {
    GOBIE,
    SHARK,
    BEAR,
    WOLF,
};

struct AnimalInTile {
    int numberIn;
    AnimalsList type;

};

struct Tile {
    Biome biome;
    int resource;
    AnimalInTile AnimalIn;
};

const int MapSize = 30;
const int MinResource = 20;
const int MaxResource = 40;

// Déclarations anticipées
class SpecieManager;
class Animals;

vector<SpecieManager> SpeciesManager;


class Map {
public:

    int animalX = 15;
    int animalY = 15;


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
        return Tiles;
    }

    void defaultMap() {
        for (int i = 0; i < MapSize; ++i) {
            for (int j = 0; j < MapSize; ++j) {
                map[i][j].biome = GRASS;
                map[i][j].resource = rand() % (MaxResource - MinResource + 1) + MinResource;
                map[i][j].AnimalIn.numberIn + 0;
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
                if (map[i][j].AnimalIn.numberIn > 0) {
                    setColorText(0);

                    if (map[i][j].AnimalIn.type == GOBIE) {
                        cout << "G";
                    }
                    else if (map[i][j].AnimalIn.type == SHARK) {
                        cout << "S";
                    }
                    if (map[i][j].AnimalIn.type == BEAR) {
                        cout << "B";
                    }
                    else if (map[i][j].AnimalIn.type == WOLF) {
                        cout << "W";
                    }
                    cout << map[i][j].AnimalIn.numberIn;
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
                        map[newX][newY].AnimalIn.numberIn = 0;
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

    void startGeneration() {
        defaultMap();
        generateWater();
        generateResources();
        generateVolcano();
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

struct Coordinates
{
    float x;
    float y;
};

class Animals {
public:
    Coordinates coords;
    float orientation = rand() % 6;
    float age = 0;
    int maxFood;
    int currentFood;
    string specie; //TODO : definir ca pour chaque esp�ce
    int dailyEat;
    float speed;
    Biome type;
    bool groupAnimal;


    Animals(
        Biome type,              // Définir le biome par défaut si nécessaire
        string specie,                // Nom de l'espèce
        int maxFood,                         // Quantité maximale de nourriture (peut être randomisé ailleurs)
        int dailyEat,                         // Quantité de nourriture consommée par jour
        float speed = 1.0f                     // Vitesse de l'animal
    ) :
        maxFood(maxFood),
        currentFood(maxFood),                     // Initialise currentFood au maximum de nourriture disponible
        specie(specie),
        dailyEat(dailyEat),
        speed(speed),
        type(type)

    {
        Map& mapp = map;
        vector<pair<int, int>> goodTiles = map.getTiles(type);
        pair<int, int> tile = goodTiles[int(rand() % goodTiles.size())];
        coords.x = tile.first;
        coords.y = tile.second;
    }

    int getFood() {
        return currentFood;
    }
    void setFood(int _food) {
        currentFood = _food;
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

    void Move(Tile map[30][30])
    {
        int temp = getFood() - 1;
        setFood(temp);

        cout << getFood() << "\n";

        if (currentFood < 0.3f * maxFood) // Si il a moins de 33% de nourriture
        {
            cout << "efkzf\n";
            if (map[int(coords.x)][int(coords.y)].resource > dailyEat) // Si y'a a manger
            {
                Eat(map);
            }
            else
            {
                int dx = round(cos(orientation));
                int dy = round(sin(orientation));

                if (map[int(coords.x) + dx][int(coords.y) + dy].resource > dailyEat) // Si la case devant lui a a manger, avancer
                {
                    coords.x += cos(orientation) * speed;
                    coords.y += sin(orientation) * speed;
                }

                else
                {
                    orientation += 1; 
                }
            }
        }
        else
        {
            float randMovement = (float((rand() % 9) + 1) / 10);
            
            float tempcoordX = cos(orientation) * speed * randMovement;
            float tempcoordY = sin(orientation) * speed * randMovement;
            cout << map[int(coords.x + tempcoordX)][int(coords.y + tempcoordY)].biome << "    " << type;
            if (map[int(coords.x + tempcoordX)][int(coords.y + tempcoordY)].biome == type)
            {
                coords.x += tempcoordX;
                coords.y += tempcoordY;
            }
            else
            {
                orientation += 1;
            }
            orientation -= (float((rand() % 9) + 1) / 10);
        }
    }

    void Eat(Tile map[30][30])
    {
        cout << "miam" << currentFood << map[int(coords.x)][int(coords.y)].resource;
        currentFood += dailyEat;
        map[int(coords.x)][int(coords.y)].resource += -dailyEat;

    }

    void CheckPositionDeath()
    {
        if (map.map[int(coords.x)][int(coords.y)].biome != type)
        {
        //detruire
            setFood(0);
        }
    }
};

class SpecieManager
{
public:
    std::vector<Animals> AnimalList;
    Tile map[30][30];
    Biome type;
    std::string specie;
    int maxFood;
    int dailyEat;
    float speed;
    bool groupAnimal;

    SpecieManager(
        Tile(&mapp)[30][30], // Passage par référence
        Biome type,
        std::string specie,
        int maxFood,
        int dailyEat,
        float speed = 1.0f,
        bool _groupAnimal = false
    ) :
        type(type),
        specie(specie),
        maxFood(maxFood),
        dailyEat(dailyEat),
        speed(speed),
        groupAnimal(_groupAnimal)
    {
        // Copie manuelle du tableau
        for (int i = 0; i < 30; ++i)
            for (int j = 0; j < 30; ++j)
                map[i][j] = mapp[i][j];
    }

    void AddAnimal()
    {
        if (groupAnimal)
        {
            AnimalList.push_back(Animals(type, specie, maxFood, dailyEat, speed));
        }
        else
        {
            AnimalList.push_back(Animals(type, specie, maxFood, dailyEat, speed));
        }
        
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

    vector<Animals> GetAnimalsArround(float x, float y, float radius = 1)
    {
        //return the number of animal of that specie arround a place, with a radius (by default radius = 1).
        // Dans une zone carr�e
        vector<Animals> closeList;
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

    void MoveAnimals(Tile map[30][30]) {

        for (auto& animal : AnimalList)
        {
            if (animal.getFood() != 0)
            {
                animal.Move(map);
            }
        }
    }

    void CheckSpeciePositionDeath()
    {
        for (auto& animal : AnimalList)
        {
            animal.CheckPositionDeath();
        }
    }
};

class Gobie : public Animals
{
public:

    Gobie(Biome _type, string _specie, int _maxFood, int _dailyEat, float _speed) : Animals(type = _type,
        specie = _specie,
        maxFood = _maxFood,
        dailyEat = _dailyEat,
        speed = _speed) {}


    Coordinates GetGroupAverageCoords(std::vector<Animals> gobieList)
    {
        float x = 0;
        float y = 0;
        for (auto gobie : gobieList)
        {
            Coordinates coords = gobie.getCoords();
            x += coords.x;
            y += coords.y;
        }
        x = x / gobieList.size();
        y = y / gobieList.size();
        return Coordinates{ x, y };
    }

    Coordinates GetDirectionVector(Coordinates coordsA, Coordinates coordsB)
    {
        //TODO Rajouter un mutiplicateur par rapport a la distance ?
        float dx = coordsB.x - coordsA.x;
        float dy = coordsB.y - coordsA.y;

        if (dx == 0 && dy == 0) {
            return { 0.0f, 0.0f };
        }

        float angle = atan2(dy, dx); //angle en radians


        float dirX = cos(angle);
        float dirY = sin(angle);

        return { dirX, dirY };
    }

    Coordinates GroupAttraction(std::vector<Animals> gobieList)
    {
        Coordinates coords = getCoords();

        Coordinates direction = GetDirectionVector(coords, GetGroupAverageCoords(gobieList));

        return direction;
    }


    float GetGroupAverageDir(std::vector<Animals> gobieList)
    {
        float dir = 0;
        for (auto gobie : gobieList)
        {
            dir += gobie.GetDirection();
        }
        dir = dir / gobieList.size();
        return dir;
    }

    Coordinates GetEntityCollisionMovement(Coordinates coordsA, Coordinates coordsB, float distTriggerMax)
    {
        //Get angle

        float dx = coordsB.x - coordsA.x;
        float dy = coordsB.y - coordsA.y;

        float angle = atan2(dy, dx); //angle en radians

        //Get Distance
        float distance;

        float multiplier = distTriggerMax - distance; // far -> less force
        //TODO

        float dirX = cos(angle) * multiplier;
        float dirY = sin(angle) * multiplier;

        return { dirX, dirY };
        // direction == Lui vers moi

    }

    Coordinates GetGroupCollisionMovement(Coordinates coords, std::vector<Animals> gobieList, float distTriggerMax)
    {
        float totalX = 0;
        float totalY = 0;
        for (auto gobie : gobieList)
        {
            Coordinates movementCoordinates = GetEntityCollisionMovement(coords, gobie.getCoords(), distTriggerMax);
            totalX += movementCoordinates.x;
            totalY += movementCoordinates.y;
        }
        return { totalX, totalY };
    }

    void MoveGroupEffect(SpecieManager GobieManager)
    {
        float distTriggerMax = 1;
        Coordinates GobieCoords = getCoords();
        std::vector<Animals> InRangeGobieList = GobieManager.GetAnimalsArround(GobieCoords.x, GobieCoords.y, distTriggerMax);

        //Exclude Himself
        for (auto gobie : InRangeGobieList)
        {
            Coordinates gobieXY = gobie.getCoords();
            if (gobieXY.x == GobieCoords.x && gobieXY.y == GobieCoords.y)
            {
                //TODO : Delete gobie from InRangeGobieList
            }
        }



        // Group Attraction
        Coordinates coords = getCoords();
        Coordinates groupAttraction = GroupAttraction(InRangeGobieList);

        coords = { coords.x + groupAttraction.x, coords.y + groupAttraction.y };


        // Group Collision avoidance
        Coordinates groupCollision = GetGroupCollisionMovement(GobieCoords, InRangeGobieList, distTriggerMax);

        coords = { coords.x + groupCollision.x, coords.y + groupCollision.y };

        SetCoords(coords.x, coords.y);


        // Group Direction Alignement
        float direction = GetDirection();
        float averageDirection = GetGroupAverageDir(InRangeGobieList);

        direction = direction * 0.9 + averageDirection * 0.1;

    }
};


bool gameOver = false;


// DEF SPECIES
SpecieManager GobieManager = SpecieManager(map.map, WATER, "Gobie", 9, 5, 1.5f, true);
SpecieManager SharkManager = SpecieManager(map.map, WATER, "Sharl", 10, 5, 2.5f, false);
SpecieManager BearManager = SpecieManager(map.map, GRASS, "Bear", 20, 10, 1.0f, false);
SpecieManager WolfManager = SpecieManager(map.map, GRASS, "Wolf", 10, 10, 4.0f, true);


void drawSpecie(vector<SpecieManager> SpecieList)
{
    vector<Coordinates> AnimalInCoords;
    for (auto specie : SpecieList)
    {
        for (auto animal : specie.AnimalList) // pour chaque animal check ou il est et le rajouter a AnimalIn
        {
            AnimalInCoords.push_back(animal.getCoords());
            cout << animal.getCoords().x << ", " << animal.getCoords().y << "\n";
        }

        for (int i = 0; i < 30; i++)
        {
            for (int j = 0; j < 30; j++)
            {
                int counter = 0;
                for (Coordinates coords : AnimalInCoords)
                {
                    if (int(coords.x) == i && int(coords.y) == j)
                    {
                        counter += 1;

                    }

                    map.map[i][j].AnimalIn.numberIn = counter;
                    
                    if (specie.specie == "Gobie") {
                        map.map[i][j].AnimalIn.type = GOBIE;
                    }
                    else if (specie.specie == "Sharl") {
                        map.map[i][j].AnimalIn.type = SHARK;
                    }
                    else if (specie.specie == "Bear") {
                        map.map[i][j].AnimalIn.type = BEAR;
                    }
                    else if (specie.specie == "Wolf") {
                        map.map[i][j].AnimalIn.type = WOLF;
                    }

                }
            }
        }
    }
}


void nextDay() {
    int daysToPass = 1;

    while (true) {
        clearScreen();
        displayLittleTitle();
        map.displayMap();
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
            map.StartFlooding();
        }
        if (x > 90) {
            map.StartVolcano();
        }
        if (map.isFlooding) {
            map.ContinueFlooding();
        }
        if (map.isUnFlooding) {
            map.ContinueUnFlooding();
        }
        if (map.isErupting) {
            map.PropagateLava();
            map.LavaToRock();
        }
        for (int i = 0; i < MapSize; ++i) {
            for (int j = 0; j < MapSize; ++j) {
                map.map[i][j].resource += rand() % 1;
            }
        }
        for (auto specie : { GobieManager, BearManager, WolfManager, SharkManager })
        {
            specie.CheckSpeciePositionDeath();
            specie.MoveAnimals(map.map);
            
        }
        //AFFICHAGE
        drawSpecie({GobieManager, BearManager, WolfManager, SharkManager});

        
        
        

        map.days++;

        //.MoveAnimal();
        clearScreen();
        displayLittleTitle();
        map.displayMap();
        cout << endl << endl << endl;
        centerText(to_string(day) + " days pass", true, 0);
        this_thread::sleep_for(chrono::seconds(1));

    }
}

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

        if (input == UP || input == Z) {
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
                nextDay();
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
                nextDay();
            }
            else if (choice == 5) {
                map.StartVolcano();
                nextDay();
            }
            else if (choice == 6) {
                map.nuke(15, 15, 16);
            }
        }
    }
}

void startGame() {

    clearScreen();
    srand((unsigned)time(NULL));
    map.startGeneration();

    //Temp
    GobieManager.AddAnimal();
    GobieManager.AddAnimal();
    GobieManager.AddAnimal();
    GobieManager.AddAnimal();

    BearManager.AddAnimal();
    BearManager.AddAnimal();
    BearManager.AddAnimal();

    WolfManager.AddAnimal();
    WolfManager.AddAnimal();
    WolfManager.AddAnimal();

    SharkManager.AddAnimal();
    SharkManager.AddAnimal();
    SharkManager.AddAnimal();

    while (!gameOver) {
        displayMenu();
    }
}
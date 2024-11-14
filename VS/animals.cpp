#include <iostream>
#include <vector>
#include "map_loader.h"
using namespace std;

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
    string specie; //TODO : définir ça pour chaque espèce
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
    
    void SetCoord(float x, float y)
    {
        coords.x = x;
        coords.y = y;
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
                    orientation += 1; // environ 60 degrés de plus
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

    void AddAnimal()
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
    
    int GetNumberAnimalsArround(float x, float y, float radius = 1)
    {
        //return the number of animal of that specie arround a place, with a radius (by default radius = 1).
        // Dans une zone carrée

        int counter = 0;
        if (AnimalList.size() == 0)
            return 0;
        for (auto animal : AnimalList)
        {
            Coordinates animalCoords = animal.getCoords();
            if ((animalCoords.x > x - radius && animalCoords.x < x + radius) &&
                (animalCoords.y > y - radius && animalCoords.y < y + radius))
            {
                counter += 1;
            }
        }
        return counter;
    }

    

    void MoveAnimals()
    {
        for (auto animal : AnimalList)
        {
            //animal.move();
            return;
        }
    }

};
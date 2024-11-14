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
    float orientation = rand() % 6;
    float age = 0;
    int maxFood;
    int currentFood;
    string specie; //TODO : definir ca pour chaque esp�ce
    int dailyEat;
    float speed;
    Biome type;


public:
    Animals(
        Map& map,
        Biome type,              // Définir le biome par défaut si nécessaire
        string specie,                // Nom de l'espèce
        int maxFood,                         // Quantité maximale de nourriture (peut être randomisé ailleurs)
        int dailyEat,                         // Quantité de nourriture consommée par jour
        float speed = 1.0f                       // Vitesse de l'animal
    ) :
        maxFood(maxFood),
        currentFood(maxFood),                     // Initialise currentFood au maximum de nourriture disponible
        specie(specie),
        dailyEat(dailyEat),
        speed(speed),
        type(type) 
    
    {
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
    Map& map;
    Biome type;    // Définir le biome par défaut si nécessaire
    string specie; // Nom de l'espèce
    int maxFood;   // Quantité maximale de nourriture (peut être randomisé ailleurs)
    int dailyEat;   // Quantité de nourriture consommée par jour
    float speed;

    SpecieManager(
        Map& map,
        Biome type,              // Définir le biome par défaut si nécessaire
        string specie,                // Nom de l'espèce
        int maxFood,                         // Quantité maximale de nourriture (peut être randomisé ailleurs)
        int dailyEat,                         // Quantité de nourriture consommée par jour
        float speed = 1.0f                       // Vitesse de l'animal
    ) :
        maxFood(maxFood),
        specie(specie),
        dailyEat(dailyEat),
        speed(speed),
        type(type),
        map(map)
    {

    }

    void AddAnimal(Map& map)
    {
        AnimalList.push_back(Animals(map, type, specie, maxFood, dailyEat, speed));
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

    void MoveAnimals(Map& map)
    {
        for (auto animal : AnimalList)
        {
            animal.Move(map);
        }
    }
};
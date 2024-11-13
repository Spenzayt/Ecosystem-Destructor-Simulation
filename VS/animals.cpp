#include <iostream>
#include <vector>
using namespace std;

struct Coordinates
{
    float x;
    float y;
};


class Animals {
private:
    Coordinates coords;
    float age;
    int foodBar;
    float direction = 90;

public :

    Animals()
    {
        age = 0;
    }
    ~Animals()
    {
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
        return direction;
    }
    void SetDirection(float dir)
    {
        direction = dir;
    }


};

class SpecieManager
{
private:
	std::vector<Animals> AnimalList;
    string name;
public:
    SpecieManager(string n) : name(n)
    {
    }

    void AddAnimal()
    {
    AnimalList.push_back(Animals());
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
        // Dans une zone carrée
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

    void MoveAnimals()
    {
        for (auto animal : AnimalList)
        {
            //animal.move();
        }
    }
    
};

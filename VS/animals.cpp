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

public :

    Animals()
    {
        age = 0;
    }
    ~Animals()
    {
        cout << "animal détruit";
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

    int GetNumberAnimalsArround(float x, float y, float radius = 1)
    {
        //return the number of animal of that specie arround a place, with a radius (by default radius = 1).
        // Dans une zone carrée

        int counter = 0;
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

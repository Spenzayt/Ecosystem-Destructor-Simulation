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
    Coordinates coord;
    float age;

public :

    Animals()
    {
        coord.x = 0; //todo: random
        coord.y = 0; //todo: random
        age = 0;
    }
    
    Coordinates getCoord()
    {
        return coord;
    }
    
    void SetCoord(float x, float y)
    {
        coord.x = x;
        coord.y = y;
    }  

    void Delete()
    {
        // todo
    }
};

class FishManager
{
private:
	std::vector<Animals> fishList;
public:
    void AddFish()
    {
    fishList.push_back(Animals());
    }

    int GetNumberFish()
    {
        return 0;// todo
    }

    void GetNumberFishArround(float x, float y, float radius) // todo: radius default = 1
    {
        //return the number of fish arround a place, with a radius (by default radius = 1).
    }

    
};

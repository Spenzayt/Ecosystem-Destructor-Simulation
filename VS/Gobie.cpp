#include <iostream>
#include "animals.cpp"
#include <cstdlib>
#include <chrono>
#include <time.h>
using namespace std;
class Gobie : public Animals
{
public:
	float direction = 0;
	
	Coordinates GetGroupAverageCoords(std::vector<Gobie> gobieList)
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

	float GetGroupAverageDir(std::vector<Gobie> gobieList)
	{
		float dir = 0;
		for (auto gobie : gobieList)
		{
			dir += gobie.direction;
		}
		dir = dir / gobieList.size();
		return dir;
	}
	
	Coordinates GetDirectionVector(Coordinates coordsA, Coordinates coordsB)
	{
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
	
	Coordinates GroupAttraction(std::vector<Gobie> gobieList)
	{
		//Return a ajouter a Move
		Coordinates coords = getCoords();

		Coordinates direction = GetDirectionVector(coords, GetGroupAverageCoords(gobieList));
		
		return direction;
	}

	void Move(SpecieManager GobieManager)
	{
		float distTriggerMax = 1;
		Coordinates GobieCoords = getCoords();
		std::vector<Animals> InRangeGobieList = GobieManager.GetAnimalsArround(GobieCoords.x, GobieCoords.y, distTriggerMax);
		
		
	}
};
#include <iostream>
#include "map_loader.h"
#include <cstdlib>
#include <chrono>
#include <time.h>
using namespace std;
class Gobie : public Animals
{
public:
	
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
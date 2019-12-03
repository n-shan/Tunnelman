#include "StudentWorld.h"
#include <string>
using namespace std;
GameWorld* createStudentWorld(string assetDir)
{ return new StudentWorld(assetDir); }
bool StudentWorld::makeEarthGrid()
//made as a bool for part 2 initialization of other objects within the field
{
	for (int i = 0; i < 60; i++)
	{
		for (int j = 0; j < 60; j++)
		{
			earthGrid[i][j].reset(new Earth(i, j));
//			earthGrid[i][j] = std::make_unique<Earth>(Earth(i, j));
			//earthGrid[i][j].reset(std::make_unique<Earth(i,j)>());
		}
	}
	for (int i = 28; i < 32; i++)
	{
		for (int j = 0; j < 60; j++)
		{
			earthGrid[i][j]->setVisible(false);
		}
		int x = 0;
	}
	return true;
}
void StudentWorld::checkOverlap(GraphObject::Direction d)
{
	int shiftX;
	int shiftY;
	switch (d)
	{
	case GraphObject::none:
		break;
	case GraphObject::up:
		shiftX = 0;
		shiftY = 1;
		break;
	case GraphObject::down:
		shiftX = 0;
		shiftY = -1;
		break;
	case GraphObject::left:
		shiftX = -1;
		shiftY = 0;
		break;
	case GraphObject::right:
		shiftX = 1;
		shiftY = 0;
		break;
	default:
		break;
	}
	if (earthGrid[Tman->getX()][Tman->getY()]->isVisible())
	{
//		earthGrid[Tman->getX()+ shiftX][Tman->getY() + shiftY]->setVisible(false);
		earthGrid[Tman->getX()][Tman->getY()]->setVisible(false);
		earthGrid[Tman->getX()+1][Tman->getY()]->setVisible(false);
		earthGrid[Tman->getX() + 2][Tman->getY()]->setVisible(false);
		earthGrid[Tman->getX()][Tman->getY()+2]->setVisible(false);
		earthGrid[Tman->getX()][Tman->getY()+1]->setVisible(false);
		earthGrid[Tman->getX()+1][Tman->getY()+1]->setVisible(false);
		earthGrid[Tman->getX() + 2][Tman->getY() + 2]->setVisible(false);
	}
	return;
}
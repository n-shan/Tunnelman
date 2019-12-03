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
//			earthGrid[i][j].reset(std::make_unique<Earth>(Earth(i,j));
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
	if (Tman->checkBounds(0, 0, 60, 60, Tman->getX(), Tman->getY(), Tman->getDirection()))
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (earthGrid[Tman->getX() + i][Tman->getY() + j]->isVisible())
					earthGrid[Tman->getX() + i][Tman->getY() + j]->setVisible(false);
			}
		}
	}
	return;
}
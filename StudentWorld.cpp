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
		for (int j = 4; j < 60; j++)
		{
			earthGrid[i][j]->setVisible(false);
		}
		int x = 0;
	}
	return true;
}
void StudentWorld::checkOverlap(GraphObject::Direction d)
{
	//checks the top four pixels of the earthGrid
	//and adjusts how much earth to remove
	int nearSurfaceShift = 4; //DO NOT FUCK WITH THE NEARSURFACESHIFT
	//asks if the Tman is in the earthGrid
	if (Tman->checkBounds(0, 0, 56, 60, Tman->getX(), Tman->getY(), GraphObject::Direction::none))
	{
		if (!Tman->checkBounds(0, 0, 56, 56, Tman->getX(), Tman->getY(), GraphObject::Direction::up))
		{
			nearSurfaceShift -= Tman->getY() - 56;
		}
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < nearSurfaceShift; j++)
			{
				if (earthGrid[Tman->getX() + i][Tman->getY() + j]->isVisible())
					earthGrid[Tman->getX() + i][Tman->getY() + j]->setVisible(false);
			}
		}

	}
	return;
}
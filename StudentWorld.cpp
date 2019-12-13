#include "StudentWorld.h"
#include <string>
#include <algorithm>
#include <cmath>
#include <cstdlib>
using namespace std;

GameWorld* createStudentWorld(string assetDir) {
	return new StudentWorld(assetDir);
}

int StudentWorld::init() {
	//display earth grid
	string s = "Scr:" + to_string(getScore());
	s += "Lvl:" + to_string(getLevel());
	s += "Lives: " + to_string(getLives());
	s += "Hlth: 000%";
	s += "Wtr: 0";
	s += "Gld: 0";
	s += "Sonar: 0";
	s += "Oil left: 0";
	setGameStatText(s);
	for (int i = 0; i < 60; i++) {
		for (int j = 0; j < 60; j++) {
			//create initial tunnel
			earthGrid[i][j].reset(new Earth(i, j, false));
			if (i < 30 || i > 33 || j < 4)
				earthGrid[i][j].reset(new Earth(i, j, true));
		}
	}
	//display boulders
	currentLevel = getLevel();
	int B = min(currentLevel / 2 + 2, 9);
	while (B)
	{
		int randomX = 30;
		int randomY = 4;
		createBoulder(randomX, randomY);
		actors.push_back(make_unique<Boulder>(randomX, randomY, this));
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				earthGrid[randomX + i][randomY + j]->setVisible(false);
			}
		}
		B--;
	}
	//display tunnelman
	tunnelMan.reset(new Tunnelman(this));
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
	// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	//return GWSTATUS_PLAYER_DIED;

	for (auto it = actors.begin(); it != actors.end(); it++)
	{
		(*it)->doSomething();
	}
	tunnelMan->doSomething();
    dig();
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() { } //all memory has garbage collection

void StudentWorld::createBoulder(int & x, int & y)
{
	x = rand() % 56;
	y = rand() % 56;
	bool InsideOtherActor = false;
	for (auto it = actors.begin(); it != actors.end(); it++)
	{
		if (!InsideOtherActor)
			InsideOtherActor = withinRadius(x, y, (*it)->getX(), (*it)->getY(), 6, 4, GraphObject::Direction::none);
		else
			break;
	}
	//while the coords are in the tunnel || in another boulder	
	if (!((x < 27 || x > 33 || y < 4)) || InsideOtherActor)
		createBoulder(x, y);
	return;
}
bool StudentWorld::withinRadius(int x, int y, int otherX, int otherY, int radius, int size, GraphObject::Direction d)
{
	int shiftX, shiftY;
	switch (d)
	{
	case GraphObject::Direction::none:
		shiftX = 0;
		shiftY = 0;
		break;
	case GraphObject::Direction::up:
		shiftX = 0;
		shiftY = 1;
		break;
	case GraphObject::Direction::down:
		shiftX = 0;
		shiftY = -1;
		break;
	case GraphObject::Direction::left:
		shiftX = -1;
		shiftY = 0;
		break;
	case GraphObject::Direction::right:
		shiftX = 1;
		shiftY = 0;
		break;
	}
	int distance = sqrt(pow(((x + (size / 2) + shiftX) - (otherX + (size / 2))),2) -
		- pow(((y + (size / 2) + shiftY) - (otherY + (size / 2))), 2));
	if (distance <= radius)
		return true;
	return false;
}
string StudentWorld::checkBounds(int boundX, int boundY, int boundShiftX, int boundShiftY, 
	int X, int Y, GraphObject::Direction d)
{
    int shiftX, shiftY;
	string s = "";
    switch (d)
    {	
	case GraphObject::Direction::none:
        shiftX = 0;
        shiftY = 0;
        break;
    case GraphObject::Direction::up:
        shiftX = 0;
        shiftY = 1;
        break;
    case GraphObject::Direction::down:
        shiftX = 0;
        shiftY = -1;
        break;
    case GraphObject::Direction::left:
        shiftX = -1;
        shiftY = 0;
        break;
    case GraphObject::Direction::right:
        shiftX = 1;
        shiftY = 0;
        break;
    default:
        return s;
    }
	if (((X + shiftX) >= boundX && (X + shiftX) <= (boundShiftX + boundX))
		|| ((Y + shiftY) >= boundY && (Y + shiftY) <= (boundShiftY + boundY)))
		s = "partial";
	else if (((X + shiftX) >= boundX && (X + shiftX) <= (boundShiftX + boundX))
        && ((Y + shiftY) >= boundY && (Y + shiftY) <= (boundShiftY + boundY)))
        s = "inside";
	else
        s = "outside";
	return s;
}
void StudentWorld::dig() {
    //dig depending on the direction
    if(tunnelMan->getDirection() == GraphObject::Direction::right) {
        clearEarth(tunnelMan->getX() + 3, tunnelMan->getY(), tunnelMan->getY(), true);
    }
    else if(tunnelMan->getDirection() == GraphObject::Direction::left) {
        clearEarth(tunnelMan->getX(), tunnelMan->getY(), tunnelMan->getY(), true);
    }
    else if(tunnelMan->getDirection() == GraphObject::Direction::up) {
        clearEarth(tunnelMan->getY() + 3, tunnelMan->getX(), tunnelMan->getY(), false);
    }
    else if(tunnelMan->getDirection() == GraphObject::Direction::down) {
        clearEarth(tunnelMan->getY(), tunnelMan->getX(), tunnelMan->getY(), false);
    }
}

void StudentWorld::clearEarth(int constLevel, int botOfLevel, int yLevel, bool isHoriz) {
    //if tunnelman is at the top of the field
    if(yLevel >= 60)
        return;
    //prevent tunnelman from digging up above y = 56
    if(yLevel > 56 && tunnelMan->getDirection() == GraphObject::Direction::up)
        return;
    int clearAmount = 4;
    //if tunnelman is near the top of the field and is horizontal
    if(yLevel > 56 && isHoriz) {
        //change amount that is cleared
        clearAmount = 60 - tunnelMan->getY();
    }
    //clear the earth
    for(int i = botOfLevel; i < botOfLevel + clearAmount ; i++) {
        if(isHoriz) {
            if(earthGrid[constLevel][i]->isVisible())
                earthGrid[constLevel][i]->setVisible(false);
        }
        else {
            if(earthGrid[i][constLevel]->isVisible())
                earthGrid[i][constLevel]->setVisible(false);
        }
    }
}
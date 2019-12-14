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
	//display Game Stats
    //setGameStatText(getStatText());
	//display earth grid
    for(int i = 0; i < 60; i++) {
        for(int j = 0; j < 60; j++) {
            //create initial tunnel
            earthGrid[i][j] = make_unique<Earth>(i, j, false);
            if(i < 30 || i > 33 || j < 4)
                earthGrid[i][j] = make_unique<Earth>(i, j, true);
        }
    }
  	//display ActivatingObjects
	currentLevel = getLevel();

	barrels = min(currentLevel + 2, 21);
	createActivatingObject(ActivatedObject(min(currentLevel / 2 + 2, 9), "Boulder"));
	createActivatingObject(ActivatedObject(barrels, "OilBarrel"));
	int B = min(currentLevel / 2 + 2, 9);
	while (B) {
		int randomX = 30;
		int randomY = 4;
		findOpenPos(randomX, randomY);
		actors.push_back(make_unique<Boulder>(std::make_shared<StudentWorld*>(this),randomX, randomY));
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				earthGrid[randomX + i][randomY + j]->setVisible(false);
			}
		}
		B--;
	}
    //gold nuggets
    int G = max(5 - currentLevel/2, 2);
    
    //display tunnelman
    tunnelMan = std::make_unique<Tunnelman>(std::make_shared<StudentWorld*>(this));
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
    // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	//display Game Stats
	setGameStatText(getStatText());
    
    int G = getLevel() * 25 + 300;
    int num = rand() % G + 1;
    //if 1/G, add sonar or water
    if(num == G) {
        //cout << "1 in G" << endl;
        num = rand() % 5 + 1;
        //add sonar
        if(num == 5) {
            //add sonar at x = 0, y = 60
        }
        //add water
        else {
            int x = rand() % 56;
            int y = rand() % 56;
            while(!canCreateAt(x, y)) {
                x = rand() % 56;
                y = rand() % 56;
            }
            createWaterPool(x, y);
        }
    }
    
	for(auto it = actors.begin(); it != actors.end(); it++) {
        if(*it != nullptr)
            (*it)->doSomething();
    }
    tunnelMan->doSomething();
    dig();
    removeDeadActors(actors);
	if (!tunnelMan->isAlive())
		return GWSTATUS_PLAYER_DIED;
	if (getBarrels() == 0)
		return GWSTATUS_FINISHED_LEVEL;
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
    for(auto it = actors.begin(); it != actors.end();) {
        it = actors.erase(it);
    }
	if(!tunnelMan->isAlive())
		decLives();
	if (getBarrels() == 0)
	{
		//Increases level in gameController 
	}
}

void StudentWorld::createActivatingObject(ActivatedObject obj) {
	while (obj.numOfObjects) {
		int randomX = 0;
		int randomY = 0;
		findOpenPos(randomX, randomY);
		if (obj.classType == "Boulder") {
			std::unique_ptr<Boulder> bldr = std::make_unique<Boulder>
				(std::make_shared<StudentWorld*>(this), randomX, randomY);
			actors.push_back(std::move(bldr));
			clearSquare(randomX, randomY);
		}
		else if (obj.classType == "OilBarrel") {
			std::cout << randomX << " " << randomY << std::endl;
			std::unique_ptr<OilBarrel> oil = std::make_unique<OilBarrel>
				(std::make_shared<StudentWorld*>(this), randomX, randomY);
			actors.push_back(std::move(oil));
		}
		obj.numOfObjects--;
	}
}
void StudentWorld::findOpenPos(int & x, int & y) {
	x = rand() % 56;
	y = rand() % 56;
	bool InsideOtherActor = false;
	for (auto it = actors.begin(); it != actors.end(); it++) {
		if (!InsideOtherActor)
			InsideOtherActor = withinRadius(x, y, (*it)->getX(), (*it)->getY(), 6, 4, GraphObject::Direction::none);
		else
			break;
	}
	//while the coords are in the tunnel || in another boulder	
	if (!((x < 27 || x > 33 || y < 4)) || InsideOtherActor)
		findOpenPos(x, y);
	return;
}
bool StudentWorld::withinRadius(int x, int y, int otherX, int otherY, int radius, int size, GraphObject::Direction d) {
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
string StudentWorld::checkBounds(int boundX, int boundY, int boundShiftX, int boundShiftY, int X, int Y, GraphObject::Direction d) {
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

std::string StudentWorld::getStatText() {
	string s = "Scr:\t " + to_string(getScore())
		+ " Lvl:\t " + to_string(getLevel())
		+ " Lives: \t " + to_string(getLives());
	s += " Hlth: \t " + to_string(tunnelMan->getHitPoints());
	s += "0%";
	s += " Wtr: \t " + to_string(tunnelMan->getWater())
		+ " Gld: \t " + to_string(tunnelMan->getGold())
		+ " Sonar: \t " + to_string(tunnelMan->getSonar())
		+ " Oil left:\t " + to_string(getBarrels());
	return s;
}
void StudentWorld::dig() {
    //dig depending on the direction
    if(tunnelMan->getDirection() == Actor::Direction::right) {
        clearEarth(tunnelMan->getX() + 3, tunnelMan->getY(), tunnelMan->getY(), true);
    }
    else if(tunnelMan->getDirection() == Actor::Direction::left) {
        clearEarth(tunnelMan->getX(), tunnelMan->getY(), tunnelMan->getY(), true);
    }
    else if(tunnelMan->getDirection() == Actor::Direction::up) {
        clearEarth(tunnelMan->getY() + 3, tunnelMan->getX(), tunnelMan->getY(), false);
    }
    else if(tunnelMan->getDirection() == Actor::Direction::down) {
        clearEarth(tunnelMan->getY(), tunnelMan->getX(), tunnelMan->getY(), false);
    }
}
void StudentWorld::clearSquare(int x, int y)
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			earthGrid[x + i][y + j]->setVisible(false);
		}
	}
}
void StudentWorld::clearEarth(int constLevel, int botOfLevel, int yLevel, bool isHoriz) {
    //if tunnelman is at the top of the field
    if(yLevel >= 60)
        return;
    //prevent tunnelman from digging up above y = 56
    if(yLevel > 56 && tunnelMan->getDirection() == Actor::Direction::up)
        return;
    int clearAmount = 4;
    //if tunnelman is near the top of the field and is horizontal
    if(yLevel > 56 && isHoriz) {
        //change amount that is cleared
        clearAmount = 60 - tunnelMan->getY();
    }
    //clear the earth
	playSound(SOUND_DIG);
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

void StudentWorld::createSquirt(int x, int y, Actor::Direction dir) {
    if(canCreateAt(x, y)) {
        std::unique_ptr<Squirt> squirt = std::make_unique<Squirt>(std::make_shared<StudentWorld*>(this), x, y, dir);
        squirt->setVisible(true);
        actors.push_back(std::move(squirt));
    }
}

bool StudentWorld::createWaterPool(int x, int y) {
    if(canCreateAt(x, y)) {
        std::unique_ptr<WaterPool> waterPool = std::make_unique<WaterPool>(std::make_shared<StudentWorld*>(this), x, y);
        actors.push_back(std::move(waterPool));
        return true;
    }
    return false;
}

//check if you can create an object at a location
bool StudentWorld::canCreateAt(int x, int y) {
    //if outside the grid
    if(x > 56 || x < 0 || y > 60 || y < 0)
        return false;
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            if(x + i >= 60 || y + j >= 60)
                continue;
            if(earthGrid[x + i][y + j]->isVisible())
                return false;
        }
    }
    return true;
}

bool StudentWorld::canMoveTo(int x, int y) {
    //if outside the grid
    if(x > 60 || x < 0 || y > 63 || y < 0 )
        return false;
    //if above grid b/w x=0,x=60
    else if((x <= 56 || x > -1) && y >= 60)
        return true;
    //if within the grid
    else
        return !earthGrid[x][y]->isVisible();
}
void StudentWorld::removeDeadActors(std::vector<std::unique_ptr<Actor>>& actors) {
    for(auto it = actors.begin(); it != actors.end();) {
        if(!(*it)->isAlive()) {
            it = actors.erase(it);
        }
        else
            it++;
    }
}

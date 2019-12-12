#include "StudentWorld.h"
#include <string>
using namespace std;

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

GameWorld* createStudentWorld(string assetDir) {
	return new StudentWorld(assetDir);
}

int StudentWorld::init() {
    //display earth grid
    for(int i = 0; i < 60; i++) {
        for(int j = 0; j < 60; j++) {
            //create initial tunnel
            earthGrid[i][j].reset(new Earth(i, j, false));
            if(i < 30 || i > 33 || j < 4)
                earthGrid[i][j].reset(new Earth(i, j, true));
        }
    }
    //display tunnelman
    tunnelMan.reset(new Tunnelman(this));
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
    // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    //return GWSTATUS_PLAYER_DIED;
    for(auto it = actors.begin(); it != actors.end(); it++) {
        if(*it != nullptr)
            (*it)->doSomething();
    }
    tunnelMan->doSomething();
    dig();
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() { } //all memory has garbage collection

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
        std::unique_ptr<Squirt> squirt = std::make_unique<Squirt>(x, y, dir, this);
        squirt->setVisible(true);
        actors.push_back(std::move(squirt));
    }
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

void StudentWorld::removeDeadActors(std::vector<std::unique_ptr<Actor>> actors) {
    
}

//bool TunnelMan::checkBounds(int boundX, int boundY, int boundShiftX, int boundShiftY, int X, int Y, Direction d) {
//    int shiftX, shiftY;
//    switch (d)
//    {
//    case none:
//        shiftX = 0;
//        shiftY = 0;
//        return true;
//    case up:
//        shiftX = 0;
//        shiftY = 1;
//        break;
//    case down:
//        shiftX = 0;
//        shiftY = -1;
//        break;
//    case left:
//        shiftX = -1;
//        shiftY = 0;
//        break;
//    case right:
//        shiftX = 1;
//        shiftY = 0;
//        break;
//    default:
//        return true;
//    }
//    if (((X + shiftX) >= boundX && (X + shiftX) <= (boundShiftX - boundX))
//        && ((Y + shiftY) >= boundY && (Y + shiftY) <= (boundShiftY - boundY)))
//        return true;
//    else
//        return false;
//}

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
    
    tunnelMan->doSomething();
    dig();
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() { } //all memory has garbage collection

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

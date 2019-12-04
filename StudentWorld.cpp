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

void StudentWorld::cleanUp() {
    //TODO
}

void StudentWorld::dig() {
    if(tunnelMan->getDirection() == GraphObject::Direction::right) {
        clearEarth(tunnelMan->getX() + 3, tunnelMan->getY(), true);
    }
    else if(tunnelMan->getDirection() == GraphObject::Direction::left) {
        clearEarth(tunnelMan->getX(), tunnelMan->getY(), true);
    }
    else if(tunnelMan->getDirection() == GraphObject::Direction::up) {
        clearEarth(tunnelMan->getY() + 3, tunnelMan->getX(), false);
    }
    else if(tunnelMan->getDirection() == GraphObject::Direction::down) {
        clearEarth(tunnelMan->getY(), tunnelMan->getX(), false);
    }
}

void StudentWorld::clearEarth(int constLevel, int botOfLevel, bool isX) {
    if(botOfLevel >= 60)
        return;
    int clearAmount = 4;
    //if tunnelman is at the top of the field
    if(botOfLevel > 56) {
        //change amount that is cleared
        clearAmount = 60 - tunnelMan->getY();
    }
    for(int i = botOfLevel; i < botOfLevel + clearAmount ; i++) {
        if(isX) {
            if(earthGrid[constLevel][i]->isVisible())
                earthGrid[constLevel][i]->setVisible(false);
        }
        else {
            if(earthGrid[i][constLevel]->isVisible())
                earthGrid[i][constLevel]->setVisible(false);
        }
    }
}

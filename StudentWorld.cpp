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
            if(i < 30 || i > 33 || j < 4)
                earthGrid[i][j].reset(new Earth(i, j));
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
        clearEarth(tunnelMan->getX() + 4, tunnelMan->getY(), tunnelMan->getY() + 4);
    }
    if(tunnelMan->getDirection() == GraphObject::Direction::left) {
        
    }
    if(tunnelMan->getDirection() == GraphObject::Direction::up) {
        
    }
    if(tunnelMan->getDirection() == GraphObject::Direction::down) {
        
    }
}

void StudentWorld::clearEarth(int x, int botY, int topY) {
    for(int i = botY; i < topY; i++) {
        earthGrid[x][i]->setVisible(false);
    }
}

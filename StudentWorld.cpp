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
    decLives();
    //return GWSTATUS_PLAYER_DIED;
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
    
}



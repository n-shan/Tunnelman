#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

//actor funtions
Actor::Actor(int imageID, int startX, int startY, Direction dir, double size, int depth) :GraphObject(imageID, startX, startY, dir, size, depth) {
    setVisible(false);
}

Actor::~Actor() {
    
}

void Actor::doSomething() { return; }

//earth functions
Earth::Earth(int x, int y) : Actor(TID_EARTH, x, y, right, .25, 3) {
    setVisible(true);
}

Earth::~Earth() {
    
}

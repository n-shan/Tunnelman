#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

//actor funtions

//earth functions

//tunnelman functions
void Tunnelman::doSomething() {
    //if(lives == 0)
        //return
    int ch;
    
    if (getWorld()->getKey(ch) == true) {
        // user hit a key this tick!
        switch (ch) {
            case KEY_PRESS_LEFT:
                //move player to the left
                moveTo(getX() - 1, getY());
                break;
            case KEY_PRESS_RIGHT:
                //move player to the right
                moveTo(getX() + 1, getY());
                break;
            case KEY_PRESS_UP:
                //move player up
                moveTo(getX(), getY() + 1);
                break;
            case KEY_PRESS_DOWN:
                //move player down
                moveTo(getX(), getY() - 1);
                break;
            case KEY_PRESS_SPACE:
                //add a Squirt in front of the player
                break;
                // etc...
                
        }
    }
}


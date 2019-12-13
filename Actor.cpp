#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

//actor funtions

//earth functions

//agent functions
bool Agent::canPickThingsUp() const {
    return false;
}

bool Agent::annoy(int amt) {
    return false;
}

//tunnelman functions
void Tunnelman::doSomething() {
    //if(lives == 0)
        //return
    int ch;
    if ((*getWorld())->getKey(ch) == true) {
        // user hit a key this tick!
        //update location and remove earth while checking bounds
        switch (ch) {
            case KEY_PRESS_LEFT:
                //move player to the left
                if(getDirection() == left) {
                    if(getX() > 0)
                        moveTo(getX() - 1, getY());
                    else
                        moveTo(getX(), getY());
                }
                else
                    setDirection(left);
                break;
            case KEY_PRESS_RIGHT:
                //move player to the right
                if(getDirection() == right) {
                    if(getX() < 56)
                        moveTo(getX() + 1, getY());
                    else
                        moveTo(getX(), getY());
                }
                else
                    setDirection(right);
                break;
            case KEY_PRESS_UP:
                //move player up
                if(getDirection() == up) {
                    if(getY() < 60)
                        moveTo(getX(), getY() + 1);
                    else
                        moveTo(getX(), getY());
                }
                else
                    setDirection(up);
                break;
            case KEY_PRESS_DOWN:
                //move player down
                if(getDirection() == down) {
                    if(getY() > 0)
                        moveTo(getX(), getY() - 1);
                    else
                        moveTo(getX(), getY());
                }
                else
                    setDirection(down);
                break;
            case KEY_PRESS_SPACE:
                //add a Squirt in front of the player
                if(getDirection() == right) {
                    (*getWorld())->createSquirt(getX() + 4, getY(), getDirection());
                }
                else if(getDirection() == left) {
                    (*getWorld())->createSquirt(getX() - 4, getY(), getDirection());
                }
                else if(getDirection() == up) {
                    (*getWorld())->createSquirt(getX(), getY() + 4, getDirection());
                }
                else if(getDirection() == down) {
                    (*getWorld())->createSquirt(getX(), getY() - 4, getDirection());
                }
                break;
                // etc...
        }
    }
}
//TODO
bool Tunnelman::annoy(int amt) {
    return false;
}
//TODO
void Tunnelman::addGold() {
    
}

//Squirt functions
void Squirt::doSomething() {
    //TODO : DAMAGE PROTESTORS, CHECK FOR BOULDERS
    //if the squirt can travel
    if(disTraveled < 4) {
        if(getDirection() == right && (*getWorld())->canMoveTo(getX() + 4, getY())) {
            moveTo(getX() + 1, getY());
        }
        else if(getDirection() == left && (*getWorld())->canMoveTo(getX() - 1, getY())) {
            moveTo(getX() - 1, getY());
        }
        else if(getDirection() == up && (*getWorld())->canMoveTo(getX(), getY() + 4)) {
            moveTo(getX(), getY() + 1);
        }
        else if(getDirection() == down && (*getWorld())->canMoveTo(getX(), getY() - 1)) {
            moveTo(getX(), getY() - 1);
        }
        else {
            setVisible(false);
            //set state to dead
            return;
        }
        disTraveled++;
    }
    //if the squirt cannot travel
    else {
        setVisible(false);
        //set state to dead
    }
}


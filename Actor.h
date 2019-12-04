#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject {
public:
    Actor(int imageID, int startX, int startY, Direction dir, double size, int depth, StudentWorld* studentWorld)
    : GraphObject(imageID, startX, startY, dir, size, depth) {
        setVisible(false);
        //allows actor to access student world
        s_World = studentWorld;
    }
    ~Actor() { } //TODO
    virtual void doSomething() = 0;
    StudentWorld* getWorld() { return s_World; }
protected:
    StudentWorld* s_World;
private:
    
};

class Earth : public Actor {
public:
    Earth(int x, int y, bool isVisible) : Actor(TID_EARTH, x, y, right, .25, 3, nullptr) {
        setVisible(isVisible);
    }
    ~Earth() { } //TODO
    virtual void doSomething() { }
private:
    
};

class Tunnelman : public Actor {
public:
    Tunnelman(StudentWorld* studentWorld) : Actor(TID_PLAYER, 30, 60, right, 1.0, 0, studentWorld) {
        setVisible(true);
    }
    ~Tunnelman() { } //TODO
    virtual void doSomething();
private:
    
};

#endif // ACTOR_H_


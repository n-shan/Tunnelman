#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject{
public:
    Actor(int imageID, int startX, int startY, Direction dir, double size, int depth);
    ~Actor();
    virtual void doSomething();
private:
    
};

class Earth : public Actor {
public:
    Earth(int x, int y);
    ~Earth();
private:
    
};

class Tunnelman : public Actor {
public:
    Tunnelman();
    ~Tunnelman();
    virtual void doSomething();
private:
    
};

#endif // ACTOR_H_


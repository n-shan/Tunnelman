#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject {
public:
    Actor(int imageID, int startX, int startY, Direction dir, double size, int depth, std::shared_ptr<StudentWorld*> studentWorld)
    : GraphObject(imageID, startX, startY, dir, size, depth) {
        setVisible(false);
        //allows actor to access student world
        s_World = studentWorld;
    }
    virtual ~Actor() { }
    virtual void doSomething() = 0;
    std::shared_ptr<StudentWorld*> getWorld() { return s_World; }
protected:
    std::shared_ptr<StudentWorld*> s_World;  //TODO : make shared ptr***
private:
    
};

class Agent : public Actor {
public:
    Agent(int imageID, int startX, int startY, Direction dir, double size, int depth, std::shared_ptr<StudentWorld*> studentWorld)
    : Actor(imageID, startX, startY, dir, size, depth, studentWorld) { }
    virtual ~Agent() { }
private:
    
};

class Earth : public Actor {
public:
    Earth(int x, int y, bool isVisible) : Actor(TID_EARTH, x, y, right, .25, 3, nullptr) {
        setVisible(isVisible);
    }
    virtual ~Earth() { }
    virtual void doSomething() { }
private:
    
};

//BOUDLER : public Actor

class Squirt : public Actor {
public:
    Squirt(int x, int y, Direction dir, std::shared_ptr<StudentWorld*> studentWorld) : Actor(TID_WATER_SPURT, x, y, dir, 1.0, 1, studentWorld) { disTraveled = 0; }
    virtual ~Squirt() { }
    virtual void doSomething();
private:
    int disTraveled;
};

//CHECK IF ACTIVATING OBJECTS NEED StudentWorld PTR
class ActivatingObject : public Actor {
public:
    ActivatingObject(int imageID, int startX, int startY, Direction dir, double size, int depth, std::shared_ptr<StudentWorld*> studentWorld, bool is_Temp)
    : Actor(imageID, startX, startY, dir, size, depth, studentWorld) { isTemp = is_Temp; }
private:
    bool isTemp;
};

class Tunnelman : public Agent {
public:
    Tunnelman(std::shared_ptr<StudentWorld*> studentWorld) : Agent(TID_PLAYER, 30, 60, right, 1.0, 0, studentWorld) {
        setVisible(true);
    }
    virtual ~Tunnelman() { }
    virtual void doSomething();
private:
    
};

class Protester : public Agent {
public:
    //Protestor(args) { }
    virtual ~Protester() { }
private:
    
};

class RegularProtester : public Protester {
public:
    //RegularProtester() { }
    virtual ~RegularProtester() { }
private:
    
};

class HardCoreProtester : public Protester {
public:
    //HardCoreProtester() { }
    virtual ~HardCoreProtester() { }
private:
    
};

class OilBarrel : public ActivatingObject {
public:
    OilBarrel(int x, int y) : ActivatingObject(TID_BARREL, x, y, right, 1.0, 2, nullptr, false) { }
    virtual ~OilBarrel() { }
    virtual void doSomething();
private:
    
};

class GoldNugget : public ActivatingObject {
public:
    GoldNugget(int x, int y, bool isVisible, bool isTemp) : ActivatingObject(TID_GOLD, x, y, right, 1.0, 2, nullptr, isTemp) { setVisible(isVisible); }
    virtual ~GoldNugget() { }
private:
    
};

class SonarKit : public ActivatingObject {
public:
    SonarKit(int x, int y) : ActivatingObject(TID_SONAR, x, y, right, 1.0, 2, nullptr, true) { setVisible(true); }
    virtual ~SonarKit() { }
private:
    
};

class WaterPool : public ActivatingObject {
public:
    WaterPool(int x, int y) : ActivatingObject(TID_WATER_POOL, x, y, right, 1.0, 2, nullptr, true) { }
    virtual ~WaterPool() { }
private:
    
};

//PART 1
//Actor class
    ///constructor
    //destructor
    ///derived from GraphObject
    ///Make itself visible via setVisible(true);
    ///virtual doSomething() (can be called by World)

//Earth class
    ///constructor
    //destructor
    ///Image ID: IMID_EARTH
    ///2D Array
        //Initial tunnel
    //Interaction with tunnelman (digging)
//TunnelMan class
    ///constructor (spawn top-middle)
    //destructor(restart level)
    ///Image ID: IMID_PLAYER
    ///Make itself visible via setVisible(true);
    //virtual doSomething() (can be called by World)
        //ALMOST DONE : Movement (WASD, Arrowkeys, NumPad Arrowkeys)
            ///Use GraphObject::moveTo() to animate changed X&Y coords

//StudentWorld class
    ///Constructor (init all member vars for proper gameplay)
    //Destructor (free any dynamically allocated memory [e.g. Earth & TunnelMan])
    //Init() method
        ///create tunnelman
        ///create earth (figure out best data structure)
            ///Anything you want to track Earth & TunnelMan
    //move() method
        ///Each tick, ask TunnelMan to doSomething()
        ///Does NOT check if TunnelMan is dead (b/c it's part 1 of project)
    //cleanUp() method
        //Same thing as destructor but program needs both cleanUp and destructor

//PART2

#endif // ACTOR_H_


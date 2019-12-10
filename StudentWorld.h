#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <memory>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld {
public:
	StudentWorld(std::string assetDir) : GameWorld(assetDir) { }
    virtual ~StudentWorld() { } //all memory has garbage collection
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    //calls clearEarth depending on tunnelman's location
    void dig();
    //clears the earth at a certain location
    void clearEarth(int constLevel, int botOther, int yLevel, bool isX);
    //creats a squirt at a certain location
    void createSquirt(int x, int y, GraphObject::Direction dir);
    //check if location is occupied
    bool locIsOccupied(int x, int y);
    //removes dead actors from oil field
    void removeDeadActors(std::vector<std::unique_ptr<Actor>> actors);
private:
    std::unique_ptr<Earth> earthGrid[60][60];
    std::unique_ptr<Tunnelman> tunnelMan;
    //all actors
    std::vector<std::unique_ptr<Actor>> actors;
};

#endif // STUDENTWORLD_H_

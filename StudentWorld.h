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
    ~StudentWorld() { } //TODO
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    void dig();
    void clearEarth(int constLevel, int botOther, int topOther, bool isX);
private:
    std::unique_ptr<Earth> earthGrid[60][60];
    std::unique_ptr<Tunnelman> tunnelMan;
    //all actors
    std::vector<std::unique_ptr<Actor>> actors;
};

#endif // STUDENTWORLD_H_

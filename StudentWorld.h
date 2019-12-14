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
    
    //check if you can create an object at a location
    bool canCreateAt(int x, int y);
    //check if location is occupied
    bool canMoveTo(int x, int y);
    //removes dead actors from oil field
    void removeDeadActors(std::vector<std::unique_ptr<Actor>>& actors);
    //Returns string with current game stats
    std::string getStatText();
    //calls clearEarth depending on tunnelman's location
    void dig();
    //clears the earth at a certain location
    void clearEarth(int constLevel, int botOther, int yLevel, bool isX);
    //creats a squirt at a certain location
    void createSquirt(int x, int y, GraphObject::Direction dir);
 
    std::string checkBounds(int boundX, int boundY, int boundShiftX, int boundShiftY, int X, int Y, GraphObject::Direction d);
    
    bool withinRadius(int x, int y, int otherX, int otherY, int radius, int size, GraphObject::Direction d);
    
    std::unique_ptr<Earth> & getEarthGridPoint(int x, int y) { return earthGrid[x][y]; }
    
    std::unique_ptr<Tunnelman> & getTunnelMan() { return tunnelMan; }
    
    std::vector<std::unique_ptr<Actor>> & getActors () { return actors; }
    
    void findOpenPos(int &, int &);
    
private:
    std::unique_ptr<Earth> earthGrid[60][60];
	//CREATE AN NEW GRID CALLED ISDUG WHICH 
	//IS A UNIQUE_PTR 2D ARRAY OF BOOLS WHERE
	//EACH EARTH THAT IS ALREADY DUG IS TRUE IN THE 2D ARRAY
    std::unique_ptr<Tunnelman> tunnelMan;
    //all actors
    std::vector<std::unique_ptr<Actor>> actors;
	int currentLevel;
};

#endif // STUDENTWORLD_H_

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
    //clears 4x4 given at a certain location
    void clearSquare(int x, int y);
    //clears the earth at a certain location
    void clearEarth(int constLevel, int botOther, int yLevel, bool isX);
    //creats a squirt at a certain location
    void createSquirt(int x, int y, GraphObject::Direction dir);
    //Checks if an actor is or will be in another actor depending on its current direction
    
    //created a water pool at a certain location
    bool createWaterPool(int x, int y);
    
    std::string checkBounds(int boundX, int boundY, int boundShiftX, int boundShiftY, int X, int Y, GraphObject::Direction d);
    //Checks if two actors are within a give radius of each other by comparing the middle of both actors
    bool withinRadius(int x, int y, int otherX, int otherY, int radius, int size, GraphObject::Direction d);
    //returns a specfic earth object in the earthGrid
    std::unique_ptr<Earth> & getEarthGridPoint(int x, int y) { return earthGrid[x][y]; }
    //returns tunnelman
    std::unique_ptr<Tunnelman> & getTunnelMan() { return tunnelMan; }
    //returns actors
    std::vector<std::unique_ptr<Actor>> & getActors () { return actors; }
    //returns number of generated barrels for the current level
    int getBarrels() { return barrels; }
    //Decreases number of barrels (for statText and level progression)
    void addBarrel(int b) { barrels += b; }
    //Returns the x and y positions of a position in the earthGrid
    //that is not already occupied by another actor
    void findOpenPos(int &, int &);
    //creates all initial activatingObjects in init using a struct
    //which provides the number of objects it wants to create,
    //and a string of what type of actor it wants it to be.
    struct ActivatedObject {
        int numOfObjects;
        std::string classType;
        ActivatedObject(int n, std::string s) {
            numOfObjects = n;
            classType = s;
        }
    };
    void createActivatingObject(ActivatedObject);
    
private:
    std::unique_ptr<Earth> earthGrid[60][60];
	//CREATE AN NEW GRID CALLED ISDUG WHICH 
	//IS A UNIQUE_PTR 2D ARRAY OF BOOLS WHERE
	//EACH EARTH THAT IS ALREADY DUG IS TRUE IN THE 2D ARRAY
    std::unique_ptr<Tunnelman> tunnelMan;
    //all actors
    std::vector<std::unique_ptr<Actor>> actors;
	int currentLevel;
	int barrels;
};

#endif // STUDENTWORLD_H_

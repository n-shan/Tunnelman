#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <memory>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{
	}

	virtual int init()
	{
        //display earth grid
        for(int i = 0; i < 60; i++) {
            for(int j = 0; j < 60; j++) {
                //create initial tunnel
                if(i < 30 || i > 33 || j < 4)
                    earthGrid[i][j].reset(new Earth(i, j));
            }
        }
        //display tunnelman
        tMan.reset(new Tunnelman);
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual int move()
	{
		// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
		// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
		decLives();
		//return GWSTATUS_PLAYER_DIED;
        return GWSTATUS_CONTINUE_GAME;
	}

	virtual void cleanUp()
	{
	}

private:
    std::unique_ptr<Earth> earthGrid[60][60];
    std::unique_ptr<Tunnelman> tMan;
};

#endif // STUDENTWORLD_H_

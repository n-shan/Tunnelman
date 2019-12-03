#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_
#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <memory>
class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir) {}
	bool makeEarthGrid();
	void checkOverlap(GraphObject::Direction);
	virtual int init()
	{
		makeEarthGrid();
		//should be last line before return
		Tman = std::make_unique<TunnelMan>(std::make_shared<StudentWorld*>(this));
		return GWSTATUS_CONTINUE_GAME;
	}
	virtual int move()
	{
		decLives();
		checkOverlap(Tman->getDirection());
		Tman->doSomething();
//		return GWSTATUS_PLAYER_DIED;
		return GWSTATUS_CONTINUE_GAME;
	}
	virtual void cleanUp()
	{
	//All unique_pointers will deallocate on their own
	}

private:
	std::unique_ptr<Earth> earthGrid[60][60];
	std::unique_ptr<TunnelMan> Tman;
};
#endif // STUDENTWORLD_H_

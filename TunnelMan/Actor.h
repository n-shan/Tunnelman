#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <algorithm>

//forward declaration to be able to create the getWorld() function
//Which lets us access StudentWorld and GameWorld functions within the Actor header and cpp files
class StudentWorld; 

//The most base class of all other game objects
//It passes in required data to the GraphObject so the game is displayed properly
class Actor : public GraphObject {
public:
	Actor(std::shared_ptr<StudentWorld*> studentWorld, int imageID, int startX, int startY, Direction dir, double size, int depth)
		: GraphObject(imageID, startX, startY, dir, size, depth) {
		setVisible(false);
		is_Alive = true; //used to track and clean any objects no longer being used
		s_World = studentWorld;
	}

	virtual ~Actor() { }

	//pure virtual because almost all actors must do something but they greatly vary in functionality
	virtual void doSomething() = 0; 


	bool isAlive() const { return is_Alive; }
	void setDead() { is_Alive = false; }
	std::shared_ptr<StudentWorld*> getWorld() { return s_World; }
	int getLevel();

	virtual bool annoy(int amt) { return false; } //TODO

protected:
	std::shared_ptr<StudentWorld*> s_World;
private:
	bool is_Alive;
};

//Simplest actor, all it does is get set to visible or not depending on other actors interaction with it
class Earth : public Actor {
public:
	Earth(int x, int y, bool isVisible) : Actor(nullptr, TID_EARTH, x, y, right, .25, 3) {
		setVisible(isVisible);
	}

	virtual ~Earth() { }

	virtual void doSomething() { }
private:

};

//Blocks Agents (see below) from traversing through boulders, can fall, and fall on Agents, effectively killing them
class Boulder : public Actor
{
public:
	Boulder(std::shared_ptr<StudentWorld*> studentWorld, int startX, int startY)
		: Actor(studentWorld, TID_BOULDER, startX, startY, down, 1.0, 1) {
		setVisible(true);
		stable = true;
		waiting = 30;
	}
	virtual ~Boulder() {}
	virtual void doSomething();
	bool shouldBoulderFall(int, int); //checks every tick if the boulder should keep falling
private:
	bool stable; //used to denote if it should fall or not
	int waiting; //timer between the switch between stable and not stable
};

//Keypress(spacebar) created object, hurts and possibly kills protesters
class Squirt : public Actor {
public:
	Squirt(std::shared_ptr<StudentWorld*> studentWorld, int x, int y, Direction dir) : Actor(studentWorld, TID_WATER_SPURT, x, y, dir, 1.0, 1) { disTraveled = 0; }

	virtual ~Squirt() { }

	virtual void doSomething();
private:
	int disTraveled; //timer/check to see when squirt should stop traveling
};

//A base class of TunnelMan, and Protester, specifying health points, and Agent specific functions
class Agent : public Actor {
public:
	Agent(std::shared_ptr<StudentWorld*> studentWorld, int imageID, int startX, int startY, Direction dir, int HP)
		: Actor(studentWorld, imageID, startX, startY, dir, 1.0, 0) {
		hitPoints = HP;
	}

	virtual ~Agent() { }

	virtual bool annoy(int amt); //TODO
	// Pick up a gold nugget
	virtual void addGold(int amt) {}

	int getHitPoints() const { return hitPoints; }

	virtual bool canPickThingsUp() const; //TODO
private:
	int hitPoints;
};

//Player's character, all the get and add functions are to track stats of player 
//and possible actions the player can do based upon those stats 
class Tunnelman : public Agent {
public:
	Tunnelman(std::shared_ptr<StudentWorld*> studentWorld)
		: Agent(studentWorld, TID_PLAYER, 30, 60, right, 10) {
		setVisible(true);
	}

	virtual ~Tunnelman() { }
	bool TunnelManInteracts(Direction d);
	virtual void doSomething();
	virtual bool annoy(int amt); //TODO

	//TODO :
	virtual void addGold(int amt) { gold += amt; }
	// Pick up a sonar kit.
    void addSonar(int amt) { sonar += amt; }
	// Pick up water.
	void addWater(int w) { water += w; }
	// Get amount of gold
	int getGold() const { return gold; }
	// Get amount of sonar charges
	int getSonar() const { return sonar; }
	// Get amount of water
	int getWater() const { return water; }

private:
	int gold = 0, sonar = 1, water = 5;
};

//Many functions to make the doSomething function easier to read logic-wise.
class Protester : public Agent {
public:
	Protester(std::shared_ptr<StudentWorld*> studentWorld, int imageID, int startX, int startY, int HP)
		: Agent(studentWorld, imageID, startX, startY, right, HP) { }

	virtual ~Protester() { }

	//plays hurt or death sounds, resets numSquaresToMoveInCurrentDirection different depending on hurt or dead
	virtual bool annoy(int amt); 
	//Checks if the Protester is: 1) facing tunnelman down a hallway 2) checking there is no earth or no boulders between them.
	//Inputting the none direction checks all directions and returns the correct one by reference
	bool correctDirection(GraphObject::Direction&);
	//Checks if there is another possible way to turn, returns how many intersections there are and the type of direction it is
	int atIntersection(GraphObject::Direction &);
	
	void changeToRandomDirection(int & randomDirection);

	void tryToMove();

	virtual void addGold();
	virtual void doSomething() {}
	void findRandomDirection(int & randomDirection);
	int calculateNumSquaresToMoveInCurrentDirection (int min, int max) { return rand() % (max - min + 1) + min; };
    //updates canMoveGrid
    void updateGrid();
    //finds direction to get out of grid
    Direction findDirectionOut();
protected:
	const int calculatedTicksToWait = std::max(0, ((3 - getLevel()) / 4));
	int	currentTicksToWait = calculatedTicksToWait, 
		shoutTimer = 15,
		numSquaresToMoveInCurrentDirection = 0,
		perpendicularTickCounter = 200;
    bool canMoveGrid[60][60];
};

class RegularProtester : public Protester {
public:
	RegularProtester(std::shared_ptr<StudentWorld*> studentWorld, int startX, int startY)
		: Protester(studentWorld, TID_PROTESTER, startX, startY, 5) {
		setVisible(true);
	}

	virtual ~RegularProtester() { }

	virtual void doSomething();
	virtual void addGold() { Protester::addGold(); }
private:

};

class HardCoreProtester : public Protester {
public:
	HardCoreProtester(std::shared_ptr<StudentWorld*> studentWorld, int startX, int startY)
		: Protester(studentWorld, TID_HARD_CORE_PROTESTER, startX, startY, 20) {
		setVisible(true);
	}

	virtual ~HardCoreProtester() { }

	virtual void doSomething();
	virtual void addGold() { Protester::addGold(); }
private:

};

class ActivatingObject : public Actor {
public:
	ActivatingObject(std::shared_ptr<StudentWorld*> studentWorld, int imageID, int startX, int startY, Direction dir, double size, int depth, bool is_Temp)
		: Actor(studentWorld, imageID, startX, startY, dir, size, depth) {
		isTemp = is_Temp;
	}

	virtual ~ActivatingObject() { }


private:
	//defines whether the obj is temp or permanent
	bool isTemp;
};

class OilBarrel : public ActivatingObject {
public:
	OilBarrel(std::shared_ptr<StudentWorld*> studentWorld, int x, int y)
		: ActivatingObject(studentWorld, TID_BARREL, x, y, right, 1.0, 2, false) {
            setVisible(false);
	}
	virtual ~OilBarrel() { }
	virtual void doSomething();
private:
};

class GoldNugget : public ActivatingObject {
public:
	GoldNugget(std::shared_ptr<StudentWorld*> studentWorld, int x, int y, bool isVisible, bool isTemp, bool canTmanPickUp)
		: ActivatingObject(studentWorld, TID_GOLD, x, y, right, 1.0, 2, isTemp) {
		setVisible(isVisible);
		tManCanPickUp = canTmanPickUp;
		if (isTemp)
			tOnField = 100;
		else
			tOnField = 0;
	}

	virtual ~GoldNugget() { }
	virtual void doSomething();
private:
	bool tManCanPickUp;
	int tOnField;
	//	bool tOnField;

};

class SonarKit : public ActivatingObject {
public:
	SonarKit(std::shared_ptr<StudentWorld*> studentWorld, int x, int y)
		: ActivatingObject(studentWorld, TID_SONAR, x, y, right, 1.0, 2, true) {
            setVisible(true);
            tOnField = std::max(100, 300 - 10 * getLevel());
	}

	virtual ~SonarKit() { }
	virtual void doSomething();
private:
    int tOnField;
};

class WaterPool : public ActivatingObject {
public:
	WaterPool(std::shared_ptr<StudentWorld*> studentWorld, int x, int y)
		: ActivatingObject(studentWorld, TID_WATER_POOL, x, y, right, 1.0, 2, true) {
		setVisible(true);
		tOnField = std::max(100, 300 - 10 * getLevel());
	}

	virtual ~WaterPool() { }
	virtual void doSomething();
private:
	int tOnField;
};

#endif // ACTOR_H_


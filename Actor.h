#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <algorithm>

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject {
public:
	Actor(std::shared_ptr<StudentWorld*> studentWorld, int imageID, int startX, int startY, Direction dir, double size, int depth)
		: GraphObject(imageID, startX, startY, dir, size, depth) {
		setVisible(false);
		is_Alive = true;
		s_World = studentWorld;
	}

	virtual ~Actor() { }

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

class Earth : public Actor {
public:
	Earth(int x, int y, bool isVisible) : Actor(nullptr, TID_EARTH, x, y, right, .25, 3) {
		setVisible(isVisible);
	}

	virtual ~Earth() { }

	virtual void doSomething() { }
private:

};

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
	bool shouldBoulderFall(int, int);
private:
	bool stable;
	int waiting;
};

class Squirt : public Actor {
public:
	Squirt(std::shared_ptr<StudentWorld*> studentWorld, int x, int y, Direction dir) : Actor(studentWorld, TID_WATER_SPURT, x, y, dir, 1.0, 1) { disTraveled = 0; }

	virtual ~Squirt() { }

	virtual void doSomething();
private:
	int disTraveled;
};

class Agent : public Actor {
public:
	Agent(std::shared_ptr<StudentWorld*> studentWorld, int imageID, int startX, int startY, Direction dir, int HP)
		: Actor(studentWorld, imageID, startX, startY, dir, 1.0, 0) {
		hitPoints = HP;
	}

	virtual ~Agent() { }

	virtual bool annoy(int amt); //TODO
	// Pick up a gold nugget
	virtual void addGold(int amt) = 0;

	int getHitPoints() const { return hitPoints; }

	virtual bool canPickThingsUp() const; //TODO
private:
	int hitPoints;
};

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
	//ADD HERE
};

class Protester : public Agent {
public:
	Protester(std::shared_ptr<StudentWorld*> studentWorld, int imageID, int HP)
		: Agent(studentWorld, imageID, 60, 60, left, HP) { }

	virtual ~Protester() { }

	virtual bool annoy(int amount);

	virtual void doSomething();
	virtual void addGold();
	bool correctDirection(Direction &);
	std::vector<std::pair<GraphObject::Direction,std::pair<int,int>>> 
		getPath(int x,int y, int destinationX, int distinationY);
	void followPath(std::vector<std::pair<GraphObject::Direction, std::pair<int, int>>> p);
//	virtual void decideMove(); //8<x<60 squares before changing direction
	//int ticksToWaitBetweenMoves = max(0, 3 – current_level_number/4);
protected:
	int calculatedTicksToWait = std::max(0,3 - getLevel()/4), currentTicksToWait = calculatedTicksToWait, shoutTimer = 15;
	std::vector<std::pair<GraphObject::Direction, std::pair<int, int>>> path;
private:
};

class RegularProtester : public Protester {
public:
	RegularProtester(std::shared_ptr<StudentWorld*> studentWorld, int startX, int startY)
		: Protester(studentWorld, TID_PROTESTER, 5) { }

	virtual ~RegularProtester() { }

	virtual void doSomething();
	virtual void addGold();
private:

};

class HardCoreProtester : public Protester {
public:
	HardCoreProtester(std::shared_ptr<StudentWorld*> studentWorld, int startX, int startY)
		: Protester(studentWorld, TID_HARD_CORE_PROTESTER, 20) { }

	virtual ~HardCoreProtester() { }

	virtual void doSomething();
	virtual void addGold();
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


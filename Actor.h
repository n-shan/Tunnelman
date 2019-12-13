#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject {
public:
    Actor( std::shared_ptr<StudentWorld*> studentWorld, int imageID, int startX, int startY, Direction dir, double size, int depth)
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

//BOUDLER : public Actor

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
    : Actor(studentWorld, imageID, startX, startY, dir, 1.0, 0) { hitPoints = HP; }
    
    virtual ~Agent() { }
    
    virtual bool annoy(int amt); //TODO
    // Pick up a gold nugget
    virtual void addGold() = 0;
    
    int getHitPoints() const  { return hitPoints; }
    
    virtual bool canPickThingsUp() const; //TODO
private:
    int hitPoints;
};

class Tunnelman : public Agent {
public:
    Tunnelman(std::shared_ptr<StudentWorld*> studentWorld)
    : Agent( studentWorld, TID_PLAYER, 30, 60, right, 10) { setVisible(true); }
    
    virtual ~Tunnelman() { }
    
    virtual void doSomething();
    virtual bool annoy(int amt); //TODO
    
    //TODO :
    virtual void addGold();
    // Pick up a sonar kit.
    void addSonar();
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
    Protester(std::shared_ptr<StudentWorld*> studentWorld, int imageID, int startX, int startY, int HP)
    : Agent(studentWorld, imageID, startX, startY, left, HP) { }
    
    virtual ~Protester() { }
    
    virtual bool annoy(int amount);
    
    virtual void doSomething();
    virtual void addGold();
private:
    
};

class RegularProtester : public Protester {
public:
    RegularProtester(std::shared_ptr<StudentWorld*> studentWorld, int imageID, int startX, int startY)
    : Protester(studentWorld, imageID, startX, startY, 5) { }
    
    virtual ~RegularProtester() { }
    
    virtual void doSomething();
    virtual void addGold();
private:
    
};

class HardCoreProtester : public Protester {
public:
    HardCoreProtester(std::shared_ptr<StudentWorld*> studentWorld, int imageID, int startX, int startY)
    : Protester(studentWorld, imageID, startX, startY, 20) { }
    
    virtual ~HardCoreProtester() { }
    
    virtual void doSomething();
    virtual void addGold();
private:
    
};

class ActivatingObject : public Actor {
public:
    ActivatingObject(std::shared_ptr<StudentWorld*> studentWorld, int imageID, int startX, int startY, Direction dir, double size, int depth, bool is_Temp)
    : Actor(studentWorld, imageID, startX, startY, dir, size, depth) { isTemp = is_Temp; }
    
    virtual ~ActivatingObject() { }
    
    
private:
    //defines whether the obj is temp or permanent
    bool isTemp;
};

class OilBarrel : public ActivatingObject {
public:
    OilBarrel(std::shared_ptr<StudentWorld*> studentWorld, int x, int y)
    : ActivatingObject(studentWorld, TID_BARREL, x, y, right, 1.0, 2, false) { }
    
    virtual ~OilBarrel() { }
    virtual void doSomething();
private:
    
};

class GoldNugget : public ActivatingObject {
public:
    GoldNugget(std::shared_ptr<StudentWorld*> studentWorld, int x, int y, bool isVisible, bool isTemp)
    : ActivatingObject(studentWorld, TID_GOLD, x, y, right, 1.0, 2, isTemp) { setVisible(isVisible); }
    
    virtual ~GoldNugget() { }
    virtual void doSomething();
private:
    
};

class SonarKit : public ActivatingObject {
public:
    SonarKit(std::shared_ptr<StudentWorld*> studentWorld, int x, int y)
    : ActivatingObject(studentWorld, TID_SONAR, x, y, right, 1.0, 2, true) { setVisible(true); }
    
    virtual ~SonarKit() { }
    virtual void doSomething();
private:
    
};

class WaterPool : public ActivatingObject {
public:
    WaterPool(std::shared_ptr<StudentWorld*> studentWorld, int x, int y)
    : ActivatingObject(studentWorld, TID_WATER_POOL, x, y, right, 1.0, 2, true) { }
    
    virtual ~WaterPool() { }
    virtual void doSomething();
private:
    
};

class Boulder : public Actor
{ 
public:
	Boulder(int x, int y, StudentWorld * studentWorld) : Actor(TID_BOULDER, x, y, down, 1.0, 1, studentWorld) 
	{ 
		setVisible(true); alive = true; stable = true; waiting = 30;
	}
	virtual ~Boulder() { alive = false; }
	virtual void doSomething();
	bool shouldBoulderFall(int, int);
private:
	bool stable, alive;
	int waiting;
};
//class Squirt : public Actor
//{
//public:
//	Squirt();
//	virtual ~Squirt() {}
//	virtual void doSomething() {}
//private:
//
//};
//class Water : public Actor
//{
//public:
//	Water();
//	virtual ~Water() {}
//	virtual void doSomething() {}
//private:
//
//};
//class Barrel : public Actor
//{
//public:
//	Barrel();
//	virtual ~Barrel() {}
//	virtual void doSomething() {}
//private:
//
//};
//class Nugget : public Actor
//{
//public:
//	Nugget();
//	virtual ~Nugget() {}
//	virtual void doSomething() {}
//private:
//
//};
//class SonarKit : public Actor
//{
//public:
//	SonarKit();
//	virtual ~SonarKit() {}
//	virtual void doSomething() {}
//private:
//
//};
//class RegularProtester : public Actor
//{
//public:
//	RegularProtester();
//	virtual ~RegularProtester() {}
//	virtual void doSomething() {}
//private:
//
//};
//class HardcoreProtester : public Actor
//{
//public:
//	HardcoreProtester();
//	virtual ~HardcoreProtester() {}
//	virtual void doSomething() {}
//private:
//
//};
//PART2
	//Boulders
	//Squirt
	//Water
	//Barrels of Oil
	//Gold Nuggets
	//Sonar Kits
	//Regular Protester
	//Hardcore Protester

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


#endif // ACTOR_H_


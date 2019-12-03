#ifndef ACTOR_H_
#define ACTOR_H_
#include "GraphObject.h"
class StudentWorld;
class Actor : public GraphObject
{
public:
	Actor(int, int, int, Direction, double, int, std::shared_ptr<StudentWorld*>);
	~Actor();
	virtual void doSomething();
	std::shared_ptr<StudentWorld*>  getWorld();
//	std::shared_ptr<StudentWorld*> sw = std::make_shared<StudentWorld*>();
	std::shared_ptr<StudentWorld*> sw;

	//hitbox function
		//hitbox function same as check bounds?
	//check bounds function (int boundX, int boundY, int boundShiftX, int boundShiftY, int X, int Y)
private:

};
class Earth : public Actor
{
public:
	Earth(int, int);
	~Earth();
private:
};
class TunnelMan : public Actor
{
public:
	TunnelMan(std::shared_ptr<StudentWorld*>);
	~TunnelMan();
	bool checkBounds(int, int, int, int, int, int, Direction);
	virtual void doSomething();
private:
};
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
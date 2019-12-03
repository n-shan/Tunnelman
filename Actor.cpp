#include "Actor.h"
#include "StudentWorld.h"
//Actor----------------------------------------------------------------------------------------
Actor::Actor(int image, int startX, int startY, Direction d, double size, int depth, std::shared_ptr<StudentWorld*> SW = std::make_shared<StudentWorld*>(nullptr))
	: GraphObject(image, startX, startY, d, size, depth)
{ 
	setVisible(false);
	sw = SW;
}
Actor::~Actor() {}
void Actor::doSomething() {}
//Earth----------------------------------------------------------------------------------------
Earth::Earth(int startX, int startY)
	: Actor(TID_EARTH, startX, startY, right, .25, 3)
{ setVisible(true); }
Earth::~Earth()
{ setVisible(false); }
//TunnelMan------------------------------------------------------------------------------------
TunnelMan::TunnelMan(std::shared_ptr<StudentWorld*> SW)
	: Actor(TID_PLAYER, 28, 60, right, 1, 0, SW)
{ 
	setVisible(true); 
	
}
TunnelMan::~TunnelMan()
{
	setVisible(false); 
	
}
std::shared_ptr<StudentWorld*>  Actor::getWorld()
{ return sw; }
bool TunnelMan::checkBounds(int boundX, int boundY, int boundShiftX, int boundShiftY, int X, int Y, Direction d)
{
	int shiftX, shiftY;
	switch (d)
	{
	case none:
		return true;
	case up:
		shiftX = 0;
		shiftY = 1;
		break;
	case down:
		shiftX = 0;
		shiftY = -1;
		break;
	case left:
		shiftX = -1;
		shiftY = 0;
		break;
	case right:
		shiftX = 1;
		shiftY = 0;
		break;
	default:
		return true;
	}
	if (((X + shiftX) >= boundX && (X + shiftX) <= (boundShiftX - boundX)) 
		&& ((Y + shiftY) >= boundY && (Y + shiftY) <= (boundShiftY - boundY)))
		return true;
	else
		return false;
}
void TunnelMan::doSomething()
{	
/*	if(checkBounds())
	{
	
	}
*/
	int ch = -1;
	if ((*getWorld())->getKey(ch) == true)
	{
		switch (ch)
		{
		default:
			break;
		case KEY_PRESS_LEFT:
			if (getDirection() == left)
				if (checkBounds(0,0,56,60,getX(),getY(), getDirection())) //checks for bounds
					moveTo(getX() - 1, getY());
				else
					break;
			else
				setDirection(left);
			break;
		case KEY_PRESS_RIGHT:
			if (getDirection() == right)
				if (checkBounds(0, 0, 56, 60, getX(), getY(), getDirection()))
					moveTo(getX() + 1, getY());
				else
					break;
			else
				setDirection(right);
			break;
		case KEY_PRESS_DOWN:
			if (getDirection() == down)
				if (checkBounds(0, 0, 56, 60, getX(), getY(), getDirection()))
					moveTo(getX(), getY() - 1);
				else
					break;
			else
				setDirection(down);
			break;
		case KEY_PRESS_UP:
			if (getDirection() == up)
				if (checkBounds(0, 0, 56, 60, getX(), getY(), getDirection()))
					moveTo(getX(), getY() + 1);
				else
					break;
			else
				setDirection(up);
			break;
//		case KEY_PRESS_ESCAPE: //check if correct
//			getWorld->decLives(); //find out what hp is and try that instead
//			getWorld->cleanUp();
//			break;
		}
	}
	//if (lives == 0)
		//return;
	//if (on earth)
		//remove earth;
			//removes 4x4 earth
			//plays digsound
	//keyTemp = getKey();
	///switch(keyTemp)
		//act accordingly
	//escape exit level (kill Tman) //DONE???
	///if(different direction)
		///update direction and return
	///if(sameDirection)
		//if you can move (aka no bolder or notOutOfMapBounds)
		//Bounds: (0,0) to (60,60)	
			///move X,Y
}
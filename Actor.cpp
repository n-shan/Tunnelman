#include "Actor.h"
#include "StudentWorld.h"

//actor funtions
int Actor::getLevel() {
	return (*getWorld())->getLevel();
}

//earth functions

//agent functions
bool Agent::canPickThingsUp() const {
	return false;
}

bool Agent::annoy(int amt) {
	if (!(getHitPoints() <= 0))
	{
		hitPoints += amt;
		return true;
	}
	else
		return false;
}

bool Agent::noBoulderInFrontOfAgent(Direction d) {
	//checks if boulder is in the way
	for (auto i = (*getWorld())->getActors().begin(); i != (*getWorld())->getActors().end(); i++)
			if ((*i)->getID() == TID_BOULDER)
				if ((*getWorld())->withinRadius(getX(), getY(), (*i)->getX(), (*i)->getY(), 3, 4, d))
					return false;
	return true;
}
//tunnelman functions
void Tunnelman::doSomething() {
	if (getHitPoints() <= 0) {
		if (isAlive())
			setDead();
		return;
	}
	int ch;
	if ((*getWorld())->getKey(ch) == true) {
		// user hit a key this tick!
		//update location and remove earth while checking bounds
		bool canMove = true;
		switch (ch) {
		case KEY_PRESS_LEFT:
			//move player to the left
			if (getDirection() == left) {
				canMove = noBoulderInFrontOfAgent(left);
				if (getX() > 0 && canMove)
					moveTo(getX() - 1, getY());
				else
					moveTo(getX(), getY());
			}
			else
				setDirection(left);
			break;
		case KEY_PRESS_RIGHT:
			//move player to the right
			if (getDirection() == right) {
				canMove = noBoulderInFrontOfAgent(right);
				if (getX() < 56 && canMove)
					moveTo(getX() + 1, getY());
				else
					moveTo(getX(), getY());
			}
			else
				setDirection(right);
			break;
		case KEY_PRESS_UP:
			//move player up
			if (getDirection() == up) {
				canMove = noBoulderInFrontOfAgent(up);
				if (getY() < 60 && canMove)
					moveTo(getX(), getY() + 1);
				else
					moveTo(getX(), getY());
			}
			else
				setDirection(up);
			break;
		case KEY_PRESS_DOWN:
			//move player down
			if (getDirection() == down) {
				canMove = noBoulderInFrontOfAgent(down);
				if (getY() > 0 && canMove)
					moveTo(getX(), getY() - 1);
				else
					moveTo(getX(), getY());
			}
			else
				setDirection(down);
			break;
		case KEY_PRESS_SPACE:
			//add a Squirt in front of the player
			if (getWater() > 0) {
				(*getWorld())->playSound(SOUND_PLAYER_SQUIRT);
				if (getDirection() == right) {
					(*getWorld())->createSquirt(getX() + 4, getY(), getDirection());
				}
				else if (getDirection() == left) {
					(*getWorld())->createSquirt(getX() - 4, getY(), getDirection());
				}
				else if (getDirection() == up) {
					(*getWorld())->createSquirt(getX(), getY() + 4, getDirection());
				}
				else if (getDirection() == down) {
					(*getWorld())->createSquirt(getX(), getY() - 4, getDirection());
				}
				addWater(-1);
			}
			break;
		case KEY_PRESS_TAB:
			if (getGold() > 0) {
				addGold(-1);
				(*getWorld())->getActors().push_back(std::move(std::make_unique<GoldNugget>
					(std::make_shared<StudentWorld*>(*getWorld()), getX(), getY(), true, true, false)));
			}
			break;
		case KEY_PRESS_ESCAPE:
			setDead();
			break;

		case 'z':
		case 'Z':
			if (getSonar() > 0) {
				addSonar(-1);
				(*getWorld())->playSound(SOUND_SONAR);
				//expose all hidden game objects within 12 from tunnelman
				for (auto it = (*getWorld())->getActors().begin(); it != (*getWorld())->getActors().end(); it++) {
					if ((*getWorld())->withinRadius(getX(), getY(), (*it)->getX(), (*it)->getY(), 12, 4, getDirection())) {
						(*it)->setVisible(true);
					}

				}
			}
		}


	}
}

////TODO
//bool Tunnelman::annoy(int amt) {
//	return false;
//}

//Squirt functions
void Squirt::doSomething() {
	//TODO : DAMAGE PROTESTORS, CHECK FOR BOULDERS
	//if the squirt can travel
	if (disTraveled < 4 && isAlive()) {
		if (getDirection() == right && (*getWorld())->canMoveTo(getX() + 4, getY())) {
			moveTo(getX() + 1, getY());
		}
		else if (getDirection() == left && (*getWorld())->canMoveTo(getX() - 1, getY())) {
			moveTo(getX() - 1, getY());
		}
		else if (getDirection() == up && (*getWorld())->canMoveTo(getX(), getY() + 4)) {
			moveTo(getX(), getY() + 1);
		}
		else if (getDirection() == down && (*getWorld())->canMoveTo(getX(), getY() - 1)) {
			moveTo(getX(), getY() - 1);
		}
		//if the squirt cannot move
		else {
			setVisible(false);
			setDead();
			return;
		}
		disTraveled++;
	}
	//if the squirt has traveled the max distance
	else {
		setVisible(false);
		setDead();
	}
}

//Boulder functions
bool Boulder::shouldBoulderFall(int x, int y) {
	int count = 0;
	bool b = false;
	if (y == 0) //if on the map floor
		return b;
	for (int i = 0; i < 4; i++) {
		if (!(*getWorld())->getEarthGridPoint(getX() + i, getY() - 1)->isVisible())
			count++;
	}
	for (auto i = (*getWorld())->getActors().begin(); i < (*getWorld())->getActors().end(); i++) {
		if ((*i)->getID() == TID_BOULDER && !((*i)->getX() == getX() && (*i)->getY() == getY())) {
			std::string check = (*getWorld())->checkBounds(
				(*i)->getX(), (*i)->getY(), 4, 4, getX(), getY(), Direction::down);
			if (!(check == "outside"))
				b = false;
		}
	}
	if (count == 4)
		b = true;
	return b;
}

void Boulder::doSomething() {
	if (isAlive()) {
		bool shouldFall = shouldBoulderFall(getX(), getY());
		if (stable) {
			if (shouldFall) {
				//if waiting is over, start to fall
				if (!waiting) {
					(*getWorld())->playSound(SOUND_FALLING_ROCK);
					stable = false;
				}
				waiting -= 1;
			}
		}
		else {
			if (shouldFall) {
				//if it can keep falling
				if (shouldBoulderFall(getX(), getY())) {
					//check if protesters are going to be hit
					for (auto i = (*getWorld())->getActors().begin(); i != (*getWorld())->getActors().end(); i++)
					{
						if ((*i)->getID() == TID_PROTESTER || (*i)->getID() == TID_HARD_CORE_PROTESTER) {
							if ((*getWorld())->withinRadius(getX(), getY(), (*i)->getX(), (*i)->getY(), 3, 4, down)) {
								(*i)->annoy(100);
							}
						}
					}
					//check if player is going to be hit
					if ((*getWorld())->withinRadius(getX(), getY(), (*getWorld())->getTunnelMan()->getX(), (*getWorld())->getTunnelMan()->getY(), 3, 4, down)) {
						(*getWorld())->getTunnelMan()->annoy(100);
					}
					moveTo(getX(), getY() - 1);
				}
				//if it hits ground
				if (shouldFall && !shouldBoulderFall(getX(), getY())) {
					setDead();
				}
			}
		}
	}
}

//OilBarrel functions
void OilBarrel::doSomething() {
	if (isAlive()) {
		if ((*getWorld())->withinRadius((*getWorld())->getTunnelMan()->getX(), (*getWorld())->getTunnelMan()->getY(), getX(), getY(), 4, 4, (*getWorld())->getTunnelMan()->getDirection())) {
			setVisible(true);
			if ((*getWorld())->withinRadius((*getWorld())->getTunnelMan()->getX(), (*getWorld())->getTunnelMan()->getY(), getX(), getY(), 3, 4, (*getWorld())->getTunnelMan()->getDirection())) {
				(*getWorld())->increaseScore(1000);
				(*getWorld())->playSound(SOUND_FOUND_OIL);
				(*getWorld())->addBarrel(-1);
				setDead();
			}
		}
	}
}

//WaterPool functions
void WaterPool::doSomething() {
	if (isAlive()) {
		if (tOnField == 0) {
			setDead();
			return;
		}
		//check if tunnelman is within 3 of the waterpool
		if ((*getWorld())->withinRadius((*getWorld())->getTunnelMan()->getX(), (*getWorld())->getTunnelMan()->getY(),
			getX(), getY(), 3, 4, (*getWorld())->getTunnelMan()->getDirection())) {
			setDead();
			(*getWorld())->playSound(SOUND_GOT_GOODIE);
			(*getWorld())->getTunnelMan()->addWater(5);
			(*getWorld())->increaseScore(100);
		}
		tOnField--;
	}
}

//GoldNugget functions
void GoldNugget::doSomething() {
	if (isAlive()) {
		//if gold is not visible and tunnelman is within 4
		if (tManCanPickUp && !isVisible() && (*getWorld())->withinRadius((*getWorld())->getTunnelMan()->getX(), (*getWorld())->getTunnelMan()->getY(), getX(), getY(), 4, 4, (*getWorld())->getTunnelMan()->getDirection())) {
			setVisible(true);
			return;
		}
		else if (tManCanPickUp && (*getWorld())->withinRadius((*getWorld())->getTunnelMan()->getX(), (*getWorld())->getTunnelMan()->getY(), getX(), getY(), 3, 4, (*getWorld())->getTunnelMan()->getDirection())) {
			setDead();
			(*getWorld())->playSound(SOUND_GOT_GOODIE);
			(*getWorld())->increaseScore(10);
			(*getWorld())->getTunnelMan()->addGold(1);
		}
		else if (!tManCanPickUp /*&& within 3 from protestor*/) {
			if (tOnField)
				tOnField--;
			if (!tOnField)
				setDead();
			(*getWorld())->playSound(SOUND_PROTESTER_FOUND_GOLD);
			//tell protester it found gold
			(*getWorld())->increaseScore(25);
		}
	}
}

//SonarKit functions
void SonarKit::doSomething() {
	if (isAlive()) {
		if (tOnField == 0) {
			setDead();
			return;
		}
		//check if tunnelman is within 3 of the sonar kit
		if ((*getWorld())->withinRadius((*getWorld())->getTunnelMan()->getX(), (*getWorld())->getTunnelMan()->getY(),
			getX(), getY(), 3, 4, (*getWorld())->getTunnelMan()->getDirection())) {
			setDead();
			(*getWorld())->playSound(SOUND_GOT_GOODIE);
			(*getWorld())->getTunnelMan()->addSonar(1);
			(*getWorld())->increaseScore(75);
		}
		tOnField--;
	}
}
//Regular Protester functions
void RegularProtester::doSomething()
{
	if (getHitPoints() > 0)
	{
		if (currentTicksToWait == 0) //if protester is able to do something this tick
		{
			//see if protester can shout at tunnelMan
			Direction dir = getDirection();
			if ((*getWorld())->withinRadius(getX(), getY(),
				(*getWorld())->getTunnelMan()->getX(), (*getWorld())->getTunnelMan()->getY(), 4, 4, none)
				&& correctDirection(dir)
				&& shoutTimer == 0)
			{
				(*getWorld())->playSound(SOUND_PROTESTER_YELL);
				(*getWorld())->getTunnelMan()->annoy(-2);
				shoutTimer = 15;
				return;
			}
			else
			{
				//check if protester has line of sight of tunnelMan
				dir = none;
				if (!(*getWorld())->withinRadius(getX(), getY(),
					(*getWorld())->getTunnelMan()->getX(), (*getWorld())->getTunnelMan()->getY(), 4, 4, none)
					&& correctDirection(dir))
				{
					setDirection(dir);
					int shiftX, shiftY;
					if (dir == left)
						shiftX = -1; shiftY = 0;
					if (dir == right)
						shiftX = 1; shiftY = 0;
					if (dir == down)
						shiftX = 0; shiftY = -1;
					if (dir == up)
						shiftX = 0; shiftY = 1;
					moveTo(getX() + shiftX, getY() + shiftY);
//TODO: see if we need next 2 lines
					numSquaresToMoveInCurrentDirection = 0;
					currentTicksToWait = calculatedTicksToWait; //reset ticks to wait for
					return;
				}
				dir = getDirection();
			}
			int randomDirection = rand() % 4;
			if (numSquaresToMoveInCurrentDirection <= 0) //if the protester should change where it is looking/going to move
			{

				findRandomDirection(randomDirection);
				if (randomDirection == 0)
					setDirection(left);
				if (randomDirection == 1)
					setDirection(right);
				if (randomDirection == 2)
					setDirection(up);
				if (randomDirection == 3)
					setDirection(down);

				//8 <= numSquaresToMoveInCurrentDirection <= 60
				numSquaresToMoveInCurrentDirection = calculateNumSquaresToMoveInCurrentDirection(8, 60);
				dir = none; //used to bypass next if statement
			}
			if (dir == none)
			{
				dir = getDirection();
				if (perpendicularTickCounter == 0)
				{
					int numOfIntersections = atIntersection(dir);
					//if protester is at an intersection in tunnel, then it tells us how many and returns the last direction it found
					if (numOfIntersections == 1)
					{
						setDirection(dir);
					}
					if (numOfIntersections == 2)
					{
						if (dir == right)
						{
							int chooseLeftOrRight = rand() & 2;
							if (chooseLeftOrRight == 0)
								setDirection(left);
							setDirection(right);
						}
						if (dir == down)
						{
							int chooseUpOrDown = rand() & 2;
							if (chooseUpOrDown == 0)
								setDirection(up);
							setDirection(down);
						}
					}
					perpendicularTickCounter = 200;
					//8 <= numSquaresToMoveInCurrentDirection <= 60
					numSquaresToMoveInCurrentDirection = calculateNumSquaresToMoveInCurrentDirection(8, 60);
				}
			}

			int shiftX, shiftY;
			if (dir == left)
				shiftX = -1; shiftY = 0;
			if (dir == right)
				shiftX = 1; shiftY = 0;
			if (dir == down)
				shiftX = 0; shiftY = -1;
			if (dir == up)
				shiftX = 0; shiftY = 1;
			if (noBoulderInFrontOfAgent(dir))
				moveTo(getX() + shiftX, getY() + shiftY);
			else
				numSquaresToMoveInCurrentDirection = 0;
			//update non-resting ticks
			numSquaresToMoveInCurrentDirection--;
			perpendicularTickCounter--;
			if (shoutTimer)
				shoutTimer--;
			//last line should be
			currentTicksToWait = calculatedTicksToWait; //reset ticks to wait for
		}
		else //update resting tick
			currentTicksToWait--;
	}
	else
	{
		//		path = getPath(getX(), getY(), 60, 60);
		//		if (!path.empty())
		//			followPath(path);
		//		else //means that there is no more path follow because they are at the destination
		setDead();
		//			(*getWorld())->increaseScore(100);
	}
}
//Hardcore Protester functions
void HardCoreProtester::doSomething()
{
	if (getHitPoints() > 0)
	{
		if (currentTicksToWait == 0) //when the ticks to wait is 0
		{
			if ((*getWorld())->withinRadius(getX(), getY(),
				(*getWorld())->getTunnelMan()->getX(), (*getWorld())->getTunnelMan()->getY(), 4, 4, none))
			{
				//each tick make a thread to compare the path. 
				//If there is a new, better path, join the thread by returning the 
				//promise to a future and set path = future;
//				path = getPath(getX(), getY(),
	//				(*getWorld())->getTunnelMan()->getX(), (*getWorld())->getTunnelMan()->getY());
//				followPath(path);
			}
			else
			{
				//chooseNextPath();
			}
			//last line should be
			currentTicksToWait = calculatedTicksToWait; //reset ticks to wait for
		}
		currentTicksToWait--;
	}
	else
	{
		//		path = getPath(getX(), getY(), 60, 60);
		//		if (!path.empty())
		//			followPath(path);
		//		else //means that there is no more path follow because they are at the destination
		setDead();
	}
}
//Protester functions
//std::vector<std::pair<GraphObject::Direction, std::pair<int, int>>>
//Protester::getPath(int x, int y, int destinationX, int destinationY)
//{
//	std::vector<std::pair<GraphObject::Direction, std::pair<int, int>>> p;
//	if (x == destinationX && y == destinationY)
//		p.clear(); return p;
//
//}
bool Protester::annoy(int amt) {
	if (!(getHitPoints() <= 0))
	{
		hitPoints += amt;
		if (!hitPoints <= 0)
		{
			(*getWorld())->playSound(SOUND_PROTESTER_ANNOYED);
			currentTicksToWait = std::max(50, 100 - getLevel() * 10);
		}
		else
		{
			(*getWorld())->playSound(SOUND_PROTESTER_GIVE_UP);
		}

		return true;
	}
	else
		return false;
}

int Protester::atIntersection(Direction& d)
{
	bool intersection1 = true;
	bool intersection2 = true;
	int numOfIntersections = 0;
	for (int i = 0; i < 4; i++)
	{
		if (d == left || d == right) //check an up or down intersection
		{
			if ((*getWorld())->getEarthGridPoint(getX() + i, getY() - 1)->isVisible() && intersection1)
				intersection1 = false;
			if ((*getWorld())->getEarthGridPoint(getX() + i, getY() + 4)->isVisible() && intersection2)
				intersection2 = false;
			if (intersection1)
			{
				d = up;
				numOfIntersections++;
			}
			if (intersection2)
			{
				d = down;
				numOfIntersections++;
			}
			return numOfIntersections;
		}
		else if (d == up || d == down) //check for a left or right intersection
		{
			if ((*getWorld())->getEarthGridPoint(getX() - 1, getY() + i)->isVisible() && intersection1)
				intersection1 = false;
			if ((*getWorld())->getEarthGridPoint(getX() + 4, getY() + i)->isVisible() && intersection1)
				intersection2 = false;
			if (intersection1)
			{
				d = left;
				numOfIntersections++;
			}
			if (intersection2)
			{
				d = right;
				numOfIntersections++;
			}
			return numOfIntersections;
		}
	}
}
void Protester::findRandomDirection(int& randomDirection)
{
	//	int sentinelRecursionCheck = 0; //should not be greater than 4 //meaning it checked all directions and they are all bad
	if (randomDirection == 0) //left
	{
		bool goodDirection = true;
		for (int i = 0; i < 4; i++)
		{
			if ((*getWorld())->getEarthGridPoint(getX() - 1, getY() + i)->isVisible())
				goodDirection = false;
		}
		for (auto it = (*getWorld())->getActors().begin(); it != (*getWorld())->getActors().end(); it++)
		{
			if ((*it)->getID() == TID_BOULDER)
			{
				if ((*it)->getX() + 4 == getX() && (*it)->getY() == getY())
					goodDirection = false;
			}
		}
		if (goodDirection)
			return;
		else
			findRandomDirection(++randomDirection);
	}
	if (randomDirection == 1) //right
	{
		bool goodDirection = true;
		for (int i = 0; i < 4; i++)
		{
			if ((*getWorld())->getEarthGridPoint(getX() + 4, getY() + i)->isVisible())
				goodDirection = false;
		}
		for (auto it = (*getWorld())->getActors().begin(); it != (*getWorld())->getActors().end(); it++)
		{
			if ((*it)->getID() == TID_BOULDER)
			{
				if ((*it)->getX() == getX() + 4 && (*it)->getY() == getY())
					goodDirection = false;
			}
		}
		if (goodDirection)
			return;
		else
			findRandomDirection(++randomDirection);
	}
	if (randomDirection == 2) //up
	{
		bool goodDirection = true;
		for (int i = 0; i < 4; i++)
		{
			if ((*getWorld())->getEarthGridPoint(getX() + i, getY() + 4)->isVisible())
				goodDirection = false;
		}
		for (auto it = (*getWorld())->getActors().begin(); it != (*getWorld())->getActors().end(); it++)
		{
			if ((*it)->getID() == TID_BOULDER)
			{
				if ((*it)->getX() == getX() && (*it)->getY() == getY() + 4)
					goodDirection = false;
			}
		}
		if (goodDirection)
			return;
		else
			findRandomDirection(++randomDirection);
	}
	if (randomDirection == 3) //down
	{
		bool goodDirection = true;
		for (int i = 0; i < 4; i++)
		{
			if ((*getWorld())->getEarthGridPoint(getX() + i, getY() - 1)->isVisible())
				goodDirection = false;
		}
		for (auto it = (*getWorld())->getActors().begin(); it != (*getWorld())->getActors().end(); it++)
		{
			if ((*it)->getID() == TID_BOULDER)
			{
				if ((*it)->getX() == getX() && (*it)->getY() + 4 == getY())
					goodDirection = false;
			}
		}
		if (goodDirection)
			return;
		else
		{
			randomDirection = 0;
			findRandomDirection(randomDirection);
		}
	}
}
bool Protester::correctDirection(GraphObject::Direction& d)
{
	//check if Tman is in the direction protester is facing
	//check if there is any earth or boulders in the way
	if (d == left)
	{
		if ((*getWorld())->getTunnelMan()->getX() < getX()
			&& (*getWorld())->getTunnelMan()->getY() == getY())
		{
			for (auto it = (*getWorld())->getActors().begin(); it != (*getWorld())->getActors().end(); it++)
			{
				if ((*it)->getID() == TID_BOULDER)
				{
					if (((*it)->getY() >= getY() || (*it)->getY() < getY() + 4)
						&& (*getWorld())->getTunnelMan()->getX() < (*it)->getX() < getX()
						&& ((*getWorld())->getTunnelMan()->getY() >= (*it)->getY()
							|| (*getWorld())->getTunnelMan()->getY() < (*it)->getY() + 4))
					{
						return false;
					}
				}
			}
			int x = getX();
			bool noEarth = true;
			while (noEarth)
			{
				for (int i = 0; i < 4; i++)
				{
					noEarth = !(*getWorld())->getEarthGridPoint(x + i, getY())->isVisible();
					if (x == 0)
						noEarth = false;
					if (!noEarth)
						break;
				}
				x--;
			}
			x++;
			if (x <= (*getWorld())->getTunnelMan()->getX())
				return false;
			else
				return true;
		}
	}
	if (d == right)
	{
		if ((*getWorld())->getTunnelMan()->getX() > getX()
			&& (*getWorld())->getTunnelMan()->getY() == getY())
		{
			for (auto it = (*getWorld())->getActors().begin(); it != (*getWorld())->getActors().end(); it++)
			{
				if ((*it)->getID() == TID_BOULDER)
				{
					if (((*it)->getY() >= getY() || (*it)->getY() < getY() + 4)
						&& (*getWorld())->getTunnelMan()->getX() > (*it)->getX() > getX()
						&& ((*getWorld())->getTunnelMan()->getY() >= (*it)->getY()
							|| (*getWorld())->getTunnelMan()->getY() < (*it)->getY() + 4))
					{
						return false;
					}
				}
			}
			int x = getX();
			bool noEarth = true;
			while (noEarth)
			{
				for (int i = 0; i < 4; i++)
				{
					noEarth = !(*getWorld())->getEarthGridPoint(x + i, getY())->isVisible();
					if (x == 60)
						noEarth = false;
					if (!noEarth)
						break;
				}
				x++;
			}
			x--;
			if (x > (*getWorld())->getTunnelMan()->getX())
				return false;
			else
				return true;
		}
	}
	if (d == down)
	{
		if ((*getWorld())->getTunnelMan()->getY() < getY()
			&& (*getWorld())->getTunnelMan()->getX() == getX())
		{
			for (auto it = (*getWorld())->getActors().begin(); it != (*getWorld())->getActors().end(); it++)
			{
				if ((*it)->getID() == TID_BOULDER)
				{
					if (((*it)->getX() >= getX() || (*it)->getX() < getX() + 4)
						&& (*getWorld())->getTunnelMan()->getY() < (*it)->getY() < getY()
						&& ((*getWorld())->getTunnelMan()->getX() >= (*it)->getX()
							|| (*getWorld())->getTunnelMan()->getX() < (*it)->getX() + 4))
					{
						return false;
					}
				}
			}
			int y = getY();
			bool noEarth = true;
			while (noEarth)
			{
				for (int i = 0; i < 4; i++)
				{
					noEarth = !(*getWorld())->getEarthGridPoint(getX(), y + i)->isVisible();
					if (y == 0)
						noEarth = false;
					if (!noEarth)
						break;
				}
				y--;
			}
			y++;
			if (y >= (*getWorld())->getTunnelMan()->getY())
				return false;
			else
				return true;
		}
	}
	if (d == up)
	{
		if ((*getWorld())->getTunnelMan()->getY() > getY()
			&& (*getWorld())->getTunnelMan()->getX() == getX())
		{
			for (auto it = (*getWorld())->getActors().begin(); it != (*getWorld())->getActors().end(); it++)
			{
				if ((*it)->getID() == TID_BOULDER)
				{
					if (((*it)->getX() >= getX() || (*it)->getX() < getX() + 4)
						&& (*getWorld())->getTunnelMan()->getY() > (*it)->getY() > getY()
						&& ((*getWorld())->getTunnelMan()->getX() >= (*it)->getX()
							|| (*getWorld())->getTunnelMan()->getX() < (*it)->getX() + 4))
					{
						return false;
					}
				}
			}
			int y = getY();
			bool noEarth = true;
			while (noEarth)
			{
				for (int i = 0; i < 4; i++)
				{
					noEarth = !(*getWorld())->getEarthGridPoint(getX(), y + i)->isVisible();
					if (y == 60)
						noEarth = false;
					if (!noEarth)
						break;
				}
				y++;
			}
			y--;
			if (y > (*getWorld())->getTunnelMan()->getY())
				return false;
			else
				return true;
		}
	}
	if (d == none)
	{
		Direction d1 = left;
		Direction d2 = right;
		Direction d3 = up;
		Direction d4 = down;

		//check all directions within 4 squares
		//then change d to be the direction where tman is
		if (correctDirection(d1))
			d = d1;
		if (correctDirection(d2))
			d = d2;
		if (correctDirection(d3))
			d = d3;
		if (correctDirection(d4))
			d = d4;
	}
	return false;
}
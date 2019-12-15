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
	return false;
}

//tunnelman functions
bool Tunnelman::TunnelManInteracts(Direction d) {
	//checks if boulder is in the way
	for (auto i = (*getWorld())->getActors().begin(); i != (*getWorld())->getActors().end(); i++) {
		{
			if ((*i)->getID() == TID_BOULDER)
				if ((*getWorld())->withinRadius(getX(), getY(), (*i)->getX(), (*i)->getY(), 3, 4, d))
					return false;
		}
	}
	return true;
}
void Tunnelman::doSomething() {
	if (getHitPoints() == 0) {
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
				canMove = TunnelManInteracts(left);
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
				canMove = TunnelManInteracts(right);
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
				canMove = TunnelManInteracts(up);
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
				canMove = TunnelManInteracts(down);
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

//TODO
bool Tunnelman::annoy(int amt) {
	return false;
}

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
								std::cout << "protester hit" << std::endl;
								// kill protester
							}
						}
					}
					//check if player is going to be hit
					if ((*getWorld())->withinRadius(getX(), getY(), (*getWorld())->getTunnelMan()->getX(), (*getWorld())->getTunnelMan()->getY(), 3, 4, down)) {
						//kill player
						std::cout << "Tman hit" << std::endl;
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
	if (getHitPoints() != 0)
	{
		if (currentTicksToWait == 0) //when the ticks to wait is 0
		{
			Direction dir = getDirection();
			if ((*getWorld())->withinRadius(getX(), getY(),
				(*getWorld())->getTunnelMan()->getX(), (*getWorld())->getTunnelMan()->getY(), 4, 4, none)
				&& correctDirection(dir)
				&& shoutTimer == 0)
			{
				(*getWorld())->playSound(SOUND_PROTESTER_YELL);
				(*getWorld())->getTunnelMan()->annoy(-2);
				shoutTimer = 15;
			}
			else
			{ 
				//check if protester can phase through wall (withinRadius 4,
			}
			//chooseNextPath();
			//last line should be
			currentTicksToWait = calculatedTicksToWait; //reset ticks to wait for
		}
		currentTicksToWait--;
		if(shoutTimer)
			shoutTimer--;
	}
	else
	{
		path = getPath(getX(), getY(), 60, 60);
		if (!path.empty())
			followPath(path);
		else //means that there is no more path follow because they are at the destination
			setDead();
	}
}
//Hardcore Protester functions
void HardCoreProtester::doSomething()
{
	if (getHitPoints() != 0)
	{
		if (currentTicksToWait == 0) //when the ticks to wait is 0
		{
			if ((*getWorld())->withinRadius(getX(), getY(),
				(*getWorld())->getTunnelMan()->getX(), (*getWorld())->getTunnelMan()->getY(), 4, 4, none))
			{
				//each tick make a thread to compare the path. 
				//If there is a new, better path, join the thread by returning the 
				//promise to a future and set path = future;
				path = getPath(getX(), getY(),
					(*getWorld())->getTunnelMan()->getX(), (*getWorld())->getTunnelMan()->getY());
				followPath(path);
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
		path = getPath(getX(), getY(), 60, 60);
		if (!path.empty())
			followPath(path);
		else //means that there is no more path follow because they are at the destination
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
bool Protester::correctDirection(GraphObject::Direction &d)
{
	//check if Tman is in the direction protester is facing
	//check if there is any earth or boulders in the way
	if (d == left)
	{
		if ((*getWorld())->getTunnelMan()->getX() < getX()
			&& (*getWorld())->getTunnelMan()->getY() >= getY() //TODO: SEE IF IT SHOULD BE == OR >=
			&& (*getWorld())->getTunnelMan()->getY() < getY() + 4)
		{
			for (auto it = (*getWorld())->getActors().begin(); it != (*getWorld())->getActors().end(); it++)
			{
				if ((*it)->getID() == TID_BOULDER)
				{
					if ((*it)->getX() < getX()
						&& (*it)->getY() >= getY()
						&& (*it)->getY() < getY() + 4)
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
			&& (*getWorld())->getTunnelMan()->getY() >= getY()
			&& (*getWorld())->getTunnelMan()->getY() < getY() + 4)
		{
			for (auto it = (*getWorld())->getActors().begin(); it != (*getWorld())->getActors().end(); it++)
			{
				if ((*it)->getID() == TID_BOULDER)
				{
					if ((*it)->getX() > getX()
						&& (*it)->getY() >= getY()
						&& (*it)->getY() < getY() + 4)
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
		if ((*getWorld())->getTunnelMan()->getX() >= getX()
			&& (*getWorld())->getTunnelMan()->getX() < getX() + 4
			&& (*getWorld())->getTunnelMan()->getY() < getY())
		{
			for (auto it = (*getWorld())->getActors().begin(); it != (*getWorld())->getActors().end(); it++)
			{
				if ((*it)->getID() == TID_BOULDER)
				{
					if ((*it)->getX() >= getX()
						&& (*it)->getX() < getX() + 4
						&& (*it)->getY() < getY())
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
		if ((*getWorld())->getTunnelMan()->getX() >= getX()
			&& (*getWorld())->getTunnelMan()->getX() > getX() + 4
			&& (*getWorld())->getTunnelMan()->getY() < getY())
		{
			for (auto it = (*getWorld())->getActors().begin(); it != (*getWorld())->getActors().end(); it++)
			{
				if ((*it)->getID() == TID_BOULDER)
				{
					if ((*it)->getX() >= getX()
						&& (*it)->getX() < getX() + 4
						&& (*it)->getY() < getY())
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
		//check all directions within 4 squares
		//then change d to be the direction where tman is

	}
	return false;
}

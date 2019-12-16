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
				for (auto it = (*getWorld())->getActors().begin(); it != (*getWorld())->getActors().end(); it++)
				{
					if ((*it)->getID() == TID_GOLD)
					{
						if ((*getWorld())->withinRadius(getX(), getY(), (*it)->getX(), (*it)->getY(), 3, 4, none))
							return;
					}
				}
				addGold(-1);
<<<<<<< Updated upstream
				(*getWorld())->getActors().push_back(std::move(std::make_unique<GoldNugget>
					(std::make_shared<StudentWorld*>(*getWorld()), getX(), getY(), true, true, false)));
=======
				(*getWorld())->getActors().push_back(
					std::make_unique<GoldNugget>(std::make_shared<StudentWorld*>(*getWorld()), getX(), getY(), true, true, false));
>>>>>>> Stashed changes
			}
			break;
		case KEY_PRESS_ESCAPE:
			setDead();
			break;
<<<<<<< Updated upstream
                
        case 'z':
        case 'Z':
                if(getSonar() > 0) {
                    addSonar(-1);
                    (*getWorld())->playSound(SOUND_SONAR);
                    //expose all hidden game objects within 12 from tunnelman
                    for (auto it = (*getWorld())->getActors().begin(); it != (*getWorld())->getActors().end(); it++) {
                        if((*getWorld())->withinRadius(getX(), getY(), (*it)->getX(), (*it)->getY(), 12, 4, getDirection())) {
                            (*it)->setVisible(true);
                        }
                    
                    }
                }
        }
        

=======

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
			break;
		}
>>>>>>> Stashed changes
	}
}

//TODO
bool Tunnelman::annoy(int amt) {
<<<<<<< Updated upstream
=======
	if (hitPoints > 0)
	{
		hitPoints += amt;
		if (hitPoints > 0)
			(*getWorld())->playSound(SOUND_PLAYER_ANNOYED);
		else
			(*getWorld())->playSound(SOUND_PLAYER_GIVE_UP);
	}
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
								std::cout << "protester hit" << std::endl;
								// kill protester
=======
								(*i)->annoy(-100);
								(*getWorld())->increaseScore(500);
>>>>>>> Stashed changes
							}
						}
					}
					//check if player is going to be hit
					if ((*getWorld())->withinRadius(getX(), getY(), (*getWorld())->getTunnelMan()->getX(), (*getWorld())->getTunnelMan()->getY(), 3, 4, down)) {
<<<<<<< Updated upstream
						//kill player
						std::cout << "Tman hit" << std::endl;
=======
						(*getWorld())->getTunnelMan()->annoy(-100);

>>>>>>> Stashed changes
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
			//(*getWorld())->playSound(SOUND_PROTESTER_FOUND_GOLD);
			//tell protester it found gold
			//(*getWorld())->increaseScore(25);
		}
	}
}

//SonarKit functions
void SonarKit::doSomething() {
<<<<<<< Updated upstream
    if(isAlive()) {
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
=======
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
			bool ifConditional = true;
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
				changeToRandomDirection(randomDirection);
				//8 <= numSquaresToMoveInCurrentDirection <= 60
				numSquaresToMoveInCurrentDirection = calculateNumSquaresToMoveInCurrentDirection(8, 60);
				ifConditional = false;
			}
			if (ifConditional)
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

			tryToMove();

			//update non-resting ticks
			numSquaresToMoveInCurrentDirection--;
			perpendicularTickCounter--;
			if (shoutTimer)
				shoutTimer--;
			//last line should be
			currentTicksToWait = calculatedTicksToWait; //reset ticks to wait for
//			tryToMove();
		}
		else //update resting tick
			currentTicksToWait--;
	}
	else
	{
	//#3
		//if (getX() == 56 && getY() == 60)
		if (getX() <= 56)
		{
			setDead();
		}
		else {
			updateGrid();
			//move one step closer to exit
			//IMPLEMENT FINDDIRECTIONOUT FUNCTION
			setDirection(findDirectionOut());
			if (getDirection() == right)
				moveTo(getX() + 1, getY());

			if (getDirection() == left)
				moveTo(getX() - 1, getY());

			if (getDirection() == up)
				moveTo(getX(), getY() + 1);

			if (getDirection() == down)
				moveTo(getX(), getY() - 1);

			return;
		}
		//		path = getPath(getX(), getY(), 60, 60);
		//		if (!path.empty())
		//			followPath(path);
		//		else //means that there is no more path follow because they are at the destination
		//setDead();
		//			(*getWorld())->increaseScore(100);
	}
}
void Protester::updateGrid() {
    for(int i = 0; i < 60; i++) {
        for(int j = 0; j < 60; j++) {
            if((*getWorld())->canCreateAt(i, j)) {
                canMoveGrid[i][j] = true;
            }
            else
                canMoveGrid[i][j] = false;
>>>>>>> Stashed changes
        }
        tOnField--;
    }
}
<<<<<<< Updated upstream
=======
void Protester::tryToMove()
{
	int randomDirection = rand() % 4;
	if (getDirection() == left) {
		if (getX() >= 0 && noBoulderInFrontOfAgent(getDirection()))
		{
			if (getX() == 0)
			{
				 changeToRandomDirection(randomDirection);
				 tryToMove();
				 return;
			}
			bool checkMove = true;
			for (int i = 0; i < 4; i++)
			{
				try
				{
				if (checkMove && (*getWorld())->getEarthGridPoint(getX() - 1, getY() + i)->isVisible())
					checkMove = false;
				}
				catch (...)
				{
					checkMove = false;
				}
			}
			if (!checkMove)
			{
				changeToRandomDirection(randomDirection);
				tryToMove();
				//				checkMove = true;
				return;
			}
			else
			{
				moveTo(getX() - 1, getY());
				return;
			}
		}
		else
			numSquaresToMoveInCurrentDirection = 0;
	}
	else if (getDirection() == right) {
		if (getX() <= 56 && noBoulderInFrontOfAgent(getDirection()))
		{
			if (getX() == 56)
			{
				changeToRandomDirection(randomDirection);
				tryToMove();
				return;
			}
			bool checkMove = true;
			for (int i = 0; i < 4; i++)
			{
				try
				{
				if (checkMove && (*getWorld())->getEarthGridPoint(getX() + 4, getY() + i)->isVisible())
					checkMove = false;
				}
				catch (...)
				{
					checkMove = false;
				}
			}
			if (!checkMove)
			{
				changeToRandomDirection(randomDirection);
				tryToMove();
				//				checkMove = true;
				return;
			}
			else
			{
				moveTo(getX() + 1, getY());
				return;
			}

		}
		else
			numSquaresToMoveInCurrentDirection = 0;
	}
	else if (getDirection() == down) {
		if (getY() >= 0 && noBoulderInFrontOfAgent(getDirection()))
		{
			if (getY() == 0)
			{
				changeToRandomDirection(randomDirection);
				tryToMove();
				return;
			}
			bool checkMove = true;
			for (int i = 0; i < 4; i++)
			{
				try
				{
				if (checkMove && (*getWorld())->getEarthGridPoint(getX() + i, getY() - 1)->isVisible())
					checkMove = false;
				}
				catch (...)
				{
					checkMove = false;
				}
			}
			if (!checkMove)
			{
				changeToRandomDirection(randomDirection);
				tryToMove();
				//				checkMove = true;
				return;
			}
			else
			{
				moveTo(getX(), getY() - 1);
				return;
			}

		}
		else
			numSquaresToMoveInCurrentDirection = 0;
	}
	else if (getDirection() == up) {
		if (getY() <= 60 && noBoulderInFrontOfAgent(getDirection()))
		{
			if (getY() == 60)
			{
				changeToRandomDirection(randomDirection);
				tryToMove();
				return;
			}
			bool checkMove = true;
			for (int i = 0; i < 4; i++)
			{
				try
				{
				if (checkMove && (*getWorld())->getEarthGridPoint(getX() + i, getY() + 4)->isVisible())
					checkMove = false;
				}
				catch (...)
				{
					checkMove = false;
				}
			}
			if (!checkMove)
			{
				changeToRandomDirection(randomDirection);
				tryToMove();
//				checkMove = true;
				return;
			}
			else
			{
				moveTo(getX(), getY() + 1);
				return;
			}
		}
		else
			numSquaresToMoveInCurrentDirection = 0;
	}
}

void Protester::changeToRandomDirection(int& randomDirection)
{
	findRandomDirection(randomDirection);
	if (randomDirection == 0)
		setDirection(left);
	else if (randomDirection == 1)
		setDirection(right);
	else if (randomDirection == 2)
		setDirection(up);
	else if (randomDirection == 3)
		setDirection(down);
}
//IMPLEMENT THIS
Actor::Direction Protester::findDirectionOut() {
    //std::queue<>
    return right;
}

//Hardcore Protester functions
void HardCoreProtester::doSomething()
{
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
			if(amt != -420)
				(*getWorld())->playSound(SOUND_PROTESTER_GIVE_UP);
			currentTicksToWait = 0;
		}

		return true;
	}
	else
		return false;
}

void Protester::addGold()
{
	for (auto i = (*getWorld())->getActors().begin(); i != (*getWorld())->getActors().end(); i++)
	{
		if ((*i)->getID() == TID_GOLD)
		{
			if ((*getWorld())->withinRadius(getX(),getY(),(*i)->getX(),(*i)->getY(),3,4,none))
			{
				(*getWorld())->playSound(SOUND_PROTESTER_FOUND_GOLD);
				if (getID() == TID_PROTESTER)
				{
					annoy(-420); //Specifically the sentinel to denote grabbing gold and thus should not play the death sound.
					(*getWorld())->increaseScore(25);
				}
				if (getID() == TID_HARD_CORE_PROTESTER)
				{
					currentTicksToWait = 0; //TODO find equation and input it
					(*getWorld())->increaseScore(50);
				}

				(*i)->setDead();
			}
		}
	}
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
	if (getY() == 60)
	{
		Direction d = getDirection();
		int intersection = atIntersection(d);
		if (intersection == 1)
		{
//			if(d == up)
//			randomDirection = 2;
			if(d == down)
			randomDirection = 3;
		}
		if (intersection == 0)
		{
			int leftOrRight = rand() % 2;
			randomDirection = leftOrRight;
		}
		return;
	}
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
			//for (auto it = (*getWorld())->getActors().begin(); it != (*getWorld())->getActors().end(); it++)
			//{
			//	if ((*it)->getID() == TID_BOULDER)
			//	{
			//		if (((*it)->getY() >= getY() || (*it)->getY() < getY() + 4)
			//			&& (*getWorld())->getTunnelMan()->getX() < (*it)->getX() < getX()
			//			&& ((*getWorld())->getTunnelMan()->getY() >= (*it)->getY()
			//				|| (*getWorld())->getTunnelMan()->getY() < (*it)->getY() + 4))
			//		{
			//			return false;
			//		}
			//	}
			//}
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
	else if (d == right)
	{
		if ((*getWorld())->getTunnelMan()->getX() > getX()
			&& (*getWorld())->getTunnelMan()->getY() == getY())
		{
			//for (auto it = (*getWorld())->getActors().begin(); it != (*getWorld())->getActors().end(); it++)
			//{
			//	if ((*it)->getID() == TID_BOULDER)
			//	{
			//		if (((*it)->getY() >= getY() || (*it)->getY() < getY() + 4)
			//			&& (*getWorld())->getTunnelMan()->getX() > (*it)->getX() > getX()
			//			&& ((*getWorld())->getTunnelMan()->getY() >= (*it)->getY()
			//				|| (*getWorld())->getTunnelMan()->getY() < (*it)->getY() + 4))
			//		{
			//			return false;
			//		}
			//	}
			//}
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
	else if (d == down)
	{
		if ((*getWorld())->getTunnelMan()->getY() < getY()
			&& (*getWorld())->getTunnelMan()->getX() == getX())
		{
			//for (auto it = (*getWorld())->getActors().begin(); it != (*getWorld())->getActors().end(); it++)
			//{
			//	if ((*it)->getID() == TID_BOULDER)
			//	{
			//		if (((*it)->getX() >= getX() || (*it)->getX() < getX() + 4)
			//			&& (*getWorld())->getTunnelMan()->getY() < (*it)->getY() < getY()
			//			&& ((*getWorld())->getTunnelMan()->getX() >= (*it)->getX()
			//				|| (*getWorld())->getTunnelMan()->getX() < (*it)->getX() + 4))
			//		{
			//			return false;
			//		}
			//	}
			//}
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
	else if (d == up)
	{
		if ((*getWorld())->getTunnelMan()->getY() > getY()
			&& (*getWorld())->getTunnelMan()->getX() == getX())
		{
			//for (auto it = (*getWorld())->getActors().begin(); it != (*getWorld())->getActors().end(); it++)
			//{
			//	if ((*it)->getID() == TID_BOULDER)
			//	{
			//		if (((*it)->getX() >= getX() || (*it)->getX() < getX() + 4)
			//			&& (*getWorld())->getTunnelMan()->getY() > (*it)->getY() > getY()
			//			&& ((*getWorld())->getTunnelMan()->getX() >= (*it)->getX()
			//				|| (*getWorld())->getTunnelMan()->getX() < (*it)->getX() + 4))
			//		{
			//			return false;
			//		}
			//	}
			//}
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
	else if (d == none)
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
>>>>>>> Stashed changes

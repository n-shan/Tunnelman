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
			if (getGold() > 0)
			{
				addGold(-1);
				(*getWorld())->getActors().push_back(std::move(std::make_unique<GoldNugget>
					(std::make_shared<StudentWorld*>(*getWorld()), getX(), getY(), true, true, false)));
			}
			break;
		case KEY_PRESS_ESCAPE:
			setDead();
			break;
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
		else {
			setVisible(false);
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

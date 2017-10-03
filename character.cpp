#include <stdio.h>
#include <stdlib.h>

#include "character.h"
#include "debug.h"
#include "npc.h"
#include "pc.h"

Character::Character()
{
	dead = false;
	turn = 0;
	
	this->_isPC = false;
}

Character::~Character()
{
}

Point Character::getLocation()
{
	return Point::getPoint(x, y);
}

void Character::getLocation(int *x, int *y)
{
	*x = this->x;
	*y = this->y;
}

void Character::setLocation(int x, int y)
{
	this->x = x;
	this->y = y;
}

int Character::getColor()
{
	return color;
}


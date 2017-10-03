#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "npc.h"

NPC::NPC() : Character()
{
	speed = rand() % 16 + 5;
	abil  = rand() % 16;
	hp    = hpmaxNaked = rand() % 16 + 60;

	// get symbol
	char symb[2];
	sprintf(symb, "%x", abil);
	this->symb = symb[0];

	mempcx = 0;
	mempcy = 0;

	// get color
	if (speed <= 8)
		color = COLOR_WHITE;
	else if (speed <= 12)
		color = COLOR_GREEN;
	else if (speed <= 16)
		color = COLOR_CYAN;
	else
		color = COLOR_MAGENTA;
}

NPC::NPC(int speed, int hp, Dice *dam, char symb, int color, int abil, string name, string desc)
{
	this->speed = speed;
	this->hp    = this->hpmaxNaked = hp;
	this->dam   = dam;
	this->symb  = symb;
	this->color = color;
	this->abil  = abil;
	this->name  = name;
	this->desc  = desc;
}

NPC::~NPC()
{
}

void NPC::getMemPCLocation(int *x, int *y)
{
	*x = mempcx;
	*y = mempcy;
}

void NPC::setMemPCLocation(int x, int y)
{
	mempcx = x;
	mempcy = y;
}


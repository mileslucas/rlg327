#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "npc.h"

NPC::NPC() : Character()
{
	_isPC = false;

	abil = rand() % 16;
	speed = 5 + rand() % 16;

	// get symbol
	char symb[2];
	sprintf(symb, "%x", abil);
	this->symb = symb[0];

	mempcx = 0;
	mempcy = 0;

	// get color
	if (100/speed >= 15)
		color = COLOR_WHITE;
	else if (100/speed >= 10)
		color = COLOR_GREEN;
	else if (100/speed >= 7)
		color = COLOR_CYAN;
	else if (100/speed >= 5)
		color = COLOR_MAGENTA;
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


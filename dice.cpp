#include <stdio.h>
#include <stdlib.h>

#include "dice.h"

ostream& operator<<(ostream& os, Dice &dice)
{
	os << dice.base << "+"
		<< dice.dice << "d"
		<< dice.side;
	return os;
}

Dice::~Dice()
{
}

Dice *Dice::parseDice(string &s)
{
	// Find the base
	size_t pos = s.find('+');
	int base = atoi(s.substr(0, pos).c_str());

	// Find number dice
	size_t posd = s.find('d');
	int dice = atoi(s.substr(pos+1, posd).c_str());
	//Find side number
	int side = atoi(s.substr(posd+1).c_str());

	return new Dice(base, dice, side);
}

int Dice::roll()
{
	return base + dice * (rand() % side + 1);
}



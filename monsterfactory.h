#ifndef MONSTERFACTORY_H
#define MONSTERFACTORY_H

#include <fstream>
#include <iostream>
#include <vector>

#include "dice.h"
#include "npc.h"


using namespace std;

class MonsterFactory
{	
	public:
		string name;
		string desc;
		char symb;
		int color;
		string abil;

		Dice *dspeed;
		Dice *dhp;
		Dice *ddam;
		
		static MonsterFactory *next(istream &is);
};

void init_mf(MonsterFactory* mf);
int get_color(string);
ostream &operator<<(ostream &os, MonsterFactory &);

#endif

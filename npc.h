#ifndef NPC_H
#define NPC_H

#include "character.h"
#include "dice.h"

class NPC : public Character
{

private:
	int mempcx;
	int mempcy;

public:
	string name;
	string desc;

	int abil;

	NPC();
	NPC(int speed, int hp, Dice *dam, char symb, int color, int abil, string name, string desc);
	~NPC();

	inline int getAbil() {return abil;}

	void getMemPCLocation(int *, int *);

	inline void setAbil(int abil) {this->abil = abil;}

	void setMemPCLocation(int x, int y);
};

#endif


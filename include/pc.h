#ifndef PC_H
#define PC_H

#include "character.h"
#include "item.h"

class PC : public Character
{
public:
	PC();
	~PC();

	NPC *attacking; // currently attacking
	vector<Item *> inventory;
	Item* equipment[MAX_EQUIP];

	char *seenDungeon;

	void clearSeenDungeon();

	inline char *getSeenDungeon() { return seenDungeon; }

	void setLocation(int x, int y);

	int getSpeed();

	int getTotalDam();

	int getTotalDamRanged();

	int getTotalDef();

	int hpmax(); // hpmax plus item hit bonus

	int wearItem(int slot);
	int takeOffItem(int slot);
	int dropItem(int slot);
	int expungeItem(int slot);
	int inspectItem(int slot);
};

extern PC *pc;

#endif


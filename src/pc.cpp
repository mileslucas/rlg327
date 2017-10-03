#include <ncurses.h>
#include <stdlib.h>

#include "core.h"
#include "debug.h"
#include "dungeon.h"
#include "pc.h"

PC::PC() : Character()
{
	speed = 10;
	symb = '@';

	seenDungeon = (char *)malloc(DUNG_W * DUNG_H);

	clearSeenDungeon();

	color = COLOR_PC;

	hp = hpmaxNaked = 100;
	mp = mpmaxNaked = 100;

	dam = new Dice(5, 5, 5);

	attacking = NULL;

	for(int i = 0; i < MAX_EQUIP; i++) {
		equipment[i] = 0;
	}

}

PC::~PC()
{
	free(seenDungeon);

	delete dam;
}

void PC::clearSeenDungeon()
{
	for (int i = 0; i < DUNG_W * DUNG_H; i++)
		seenDungeon[i] = ' ';
}

void PC::setLocation(int x, int y)
{
	Character::setLocation(x, y);

	pcx = x;
	pcy = y;

	for (int r = 0; r < DUNG_H; r++) {
		for (int c = 0; c < DUNG_W; c++) {
			if (dungeon->isVisible(c, r))
				seenDungeon[r * DUNG_W + c] = dungeon->tmap[r][c];
		}
	}
}

int PC::getSpeed()
{
	int total = Character::getSpeed();
	// TODO
	return total;
}

int PC::getTotalDam()
{
	int total = dam->roll();
	// TODO
	return total;
}

int PC::getTotalDef()
{
	int total = 0;
	// TODO
	return total;
}

int PC::hpmax()
{
	return hpmaxNaked;
}

int PC::wearItem(int slot)
{
	// Check bounds for slot
	if(slot < 0 || slot > (int)inventory.size() - 1) {
		return -1;
	}

	Item *it = inventory[slot];
	if (!it->isEquipment()) {
		Debug::log("Cannot equip non-equipment type!\n");
		return -1;
	}
	// Assign item to equipment
	//Find item type and corresponding slot
	if (equipment[it->type]) {

	}

	// Remove item from inventory
	inventory.erase(inventory.begin() + slot);

	return 0;
}
int PC::takeOffItem(int slot)
{
	// TODO
	return 0;
}
int PC::dropItem(int slot)
{
	// TODO
	return 0;
}
int PC::expungeItem(int slot)
{
	// TODO
	return 0;
}
int PC::inspectItem(int slot)
{
	// TODO
	return 0;
}

// singleton PC
PC *pc;

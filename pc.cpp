#include <ncurses.h>
#include <stdlib.h>

#include "core.h"
#include "debug.h"
#include "dungeon.h"
#include "pc.h"

PC::PC() : Character()
{
	_isPC = true;

	speed = 10;
	symb = '@';

	seenDungeon = (char *)malloc(DUNG_W * DUNG_H);

	clearSeenDungeon();

	color = COLOR_PC;
	
	attacking = NULL; 
}

PC::~PC()
{
	free(seenDungeon);
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
	
	// update seen dungeon
	for (int r=0; r<DUNG_H; r++) {
		for (int c=0; c<DUNG_W; c++) {
			if (dungeon->isVisible(c, r))
				seenDungeon[r*DUNG_W+c]=dungeon->tmap[r][c];
		}
	}
}

// singleton pc
PC *pc;

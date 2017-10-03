#ifndef CHARACTER_H
#define CHARACTER_H

#include "core.h"

typedef struct
{
	// char representation of this character
	char c;
	unsigned char x, y;
	char *att;
	int speed;
	int move;

} Character;

Character **mons;
int nmons;
extern Character *pc;
int pc_is_alive;

/* character map */
Character *cmap[DUNG_H][DUNG_W];

/* inits cmap */
int cmap_init();

/* create a character */
Character *character_new(char c);

/* delete a character */
int character_delete(Character*);

/* randomly place the character in the dungeon */
int character_place(Character*);

/* moves character in dungeon */
int character_move(Character*, int, int);

/* places all monsters */
int place_monsters(int);

#endif


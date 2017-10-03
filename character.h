#ifndef CHARACTER_H
#define CHARACTER_H

#include "core.h"

#define PC_C 0x10 // PC characteristic
#define ISPC(cp) (cp->c & PC_C)

typedef struct
{	
	/* characteristics */
	char c;

	/* moving speed */
	int speed;

	/* character location */
	int x;
	int y;

	/* memorized PC location */
	int mempcx;
	int mempcy;

	/* 1 if dead */
	int dead;

	/* next turn */
	int turn;

} Character;

extern Character *pc;
extern Character *npcs[];
extern int nummon;

Character *cmap[DUNG_H][DUNG_W];

/* create a character */
Character *character_new(int isPC);

/* delete a character and update npcs and nummon */
int character_delete(Character*);

/* char representation of the character */
char character_char(Character*);

/* randomly place the character on the dungeon */
int character_place(Character*);

/* prints all the monsters on the dungeon */
int print_characters(void);

#endif


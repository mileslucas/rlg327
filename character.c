#include "core.h"
#include "character.h"
#include "dungeon.h"

Character *character_new(char ch)
{
	Character *c = malloc(sizeof(*c));
	c->c = ch;
	return c;
}

int cmap_init(void) {
	int i, j;
	for(i = 0; i < DUNG_H; i ++) {
		for(j = 0; j < DUNG_W; j++) {
			cmap[i][j] = NULL;
		}
	}
	return 0;
}

int character_delete(Character *c)
{
	free(c);
	return 0;
}

int character_place(Character *c)
{
	while(1)
	{
		int x = rand() % DUNG_W;
		int y = rand() % DUNG_H;

		if (tmap[y][x]==ROOM && cmap[y][x] == NULL)
		{
			cmap[y][x] = c;
			
			c->x = x;
			c->y = y;
			break;
		}
	}
	return 0;
}

int character_move(Character *c, int x, int y)
{
	int i;
	cmap[c->y][c->x] = NULL;
	if (cmap[y][x] != NULL) {

		if(cmap[y][x] == pc && c != pc) {
			pc_is_alive = 0;
		} else {
			for (i = 0; i < nmons; i++) {
				/* takes the monster at the end and puts it in the deleted monsters spot */
				if (cmap[y][x]->c == mons[i]->c && y == mons[i]->y && x == mons[i]->x) {
					mons[i] = mons[nmons - 1];
					nmons--;
					break;
				}
			}
		}
	}
	cmap[y][x] = c;
	c->y = y;
	c->x = x;

	return 0;
}

int place_monsters(int max)
{

	mons = malloc(sizeof(Character) * max);

	for (nmons = 0; nmons < max; nmons++)
	{
		// Character *m = character_new(HEX[rand() % 16]);
		Character *m = character_new('8');
		character_place(m);
		m->move = 0;
		m->speed = rand() % 16 + 5;
		mons[nmons] = m;
	}

	return 0;
}



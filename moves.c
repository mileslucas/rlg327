#include "dijkstra.h"
#include "core.h"
#include "character.h"
#include "dungeon.h"
#include "room.h"


int tunnel(Character *c, int x, int y) {


	if (hmap[y][x] <= 85){
		hmap[y][x] = 0;
		character_move(c, x, y);
		/* updates map if char tunnels through rock */
		tmap[c->y][c->x] = CORR;
		dijkstra_init(pc->x, pc->y, 0);
	} else {
		hmap[y][x] -= 85;
	}
	dijkstra_init(pc->x, pc->y, 1);

	return 0;
}

int move_rand(Character *c, int tun) {
	int dx,dy;
	if (tun) {
		do {
			dx = rand() % 3 - 1;
			dy = rand() % 3 - 1;
		} while (hmap[c->y + dy][c->x + dx] == IMMUTABLE);
		if (tmap[c->y + dy][c->x + dx] == ROCK) {
			tunnel(c, c->x + dx, c->y + dy);
		} else {
			character_move(c, c->x + dx, c->y + dy);
		}
	} else {
		do {
			dx = rand() % 3 - 1;
			dy = rand() % 3 - 1;
		} while (hmap[c->y + dy][c->x + dx] != 0);
		character_move(c, c->x + dx, c->y + dy);
	}

	return 0;
}

/* unintelligent, telepathic, non-tunneling, non-erratic */
int move_0100(Character *c)
{
		// move directly to pc without tunneling
		int dx, dy;

		//NW
		if(pc->x - c->x < 0 && pc->y - c->y < 0) {
			dx = -1;
			dy = -1;
		} // N 
		else if (pc->x - c->x == 0 && pc->y - c->y < 0) {
			dx = 0;
			dy = -1;
		} // NE 
		else if (pc->x - c->x > 0 && pc->y - c->y < 0) {
			dx = 1;
			dy = -1;
		} // E 
		else if (pc->x - c->x > 0 && pc->y - c->y == 0) {
			dx = 1;
			dy = 0;
		} // SE 
		else if (pc->x - c->x > 0 && pc->y - c->y > 0) {
			dx = 1;
			dy = 1;
		}  // S 
		else if (pc->x - c->x == 0 && pc->y - c->y > 0) {
			dx = 0;
			dy = 1;
		} // SW 
		else if (pc->x - c->x < 0 && pc->y - c->y > 0) {
			dx = -1;
			dy = 1;
		} // W 
		else if (pc->x - c->x < 0 && pc->y - c->y == 0) {
			dx = -1;
			dy = 0;
		}

		character_move(c, c->x + dx, c->y + dy);
	return 0;
}

/* unintelligent, non-telepathic, non-tunneling, non-erratic */
int move_0000(Character *c)
{
	// find the rooms of the character and the pc
	int i, cr, pcr;
	for (i = 0; i < ROOM_MAX_COUNT; ++i)
	{
		Room r = rooms[i];
		if ((c->y - r.y >= 0 && c->y - r.y <= r.h) &&
			(c->x - r.x >= 0 && c->x - r.x <= r.w)) {
			cr = i;
		}

		if ((pc->y - r.y >= 0 && pc->y - r.y <= r.h) &&
			(pc->x - r.x >= 0 && pc->x - r.x <= r.w)) {
			pcr = i;
		}
	}

	if(cr == pcr) {	// If in same room
		move_0100(c);
	} // Move random
	else {
		move_rand(c, 0);
	}
	return 0;
}

/* unintelligent, non-telepathic, non-tunneling, erratic */
int move_0001(Character *c)
{
	if (rand() % 2) {
		move_0000(c);
	} else {
		move_rand(c, 0);
	}
	return 0;
}

/* unintelligent, non-telepathic, tunneling, non-erratic */
int move_0010(Character *c)
{
	// find the rooms of the character and the pc
	int i, cr, pcr;
	for (i = 0; i < ROOM_MAX_COUNT; ++i)
	{
		Room r = rooms[i];
		if ((c->y - r.y >= 0 && c->y - r.y <= r.h) &&
			(c->x - r.x >= 0 && c->x - r.x <= r.w)) {
			cr = i;
		}

		if ((pc->y - r.y >= 0 && pc->y - r.y <= r.h) &&
			(pc->x - r.x >= 0 && pc->x - r.x <= r.w)) {
			pcr = i;
		}
	}

	if(room_collide(rooms[cr], rooms[pcr])) {	// If in same room
		// move directly to pc with tunneling
		int dx, dy;

		//NW
		if(pc->x - c->x < 0 && pc->y - c->y < 0) {
			dx = -1;
			dy = -1;
		} // N 
		else if (pc->x - c->x == 0 && pc->y - c->y < 0) {
			dx = 0;
			dy = -1;
		} // NE 
		else if (pc->x - c->x > 0 && pc->y - c->y < 0) {
			dx = 1;
			dy = -1;
		} // E 
		else if (pc->x - c->x > 0 && pc->y - c->y == 0) {
			dx = 1;
			dy = 0;
		} // SE 
		else if (pc->x - c->x > 0 && pc->y - c->y > 0) {
			dx = 1;
			dy = 1;
		}  // S 
		else if (pc->x - c->x == 0 && pc->y - c->y > 0) {
			dx = 0;
			dy = 1;
		} // SW 
		else if (pc->x - c->x < 0 && pc->y - c->y > 0) {
			dx = -1;
			dy = 1;
		} // W 
		else if (pc->x - c->x < 0 && pc->y - c->y == 0) {
			dx = -1;
			dy = 0;
		}
		
		character_move(c, c->x + dx, c->y + dy);
	}
	// Move random
	else {
		move_rand(c, 1);
	}
	return 0;
}


/* unintelligent, non-telepathic, tunneling, erratic */
int move_0011(Character *c)
{
	if (rand() % 2) {
		move_0010(c);
	} else {
		move_rand(c, 1);
	}
	return 0;
}



/* unintelligent, telepathic, non-tunneling, erratic */
int move_0101(Character *c)
{
	if (rand() % 2) {
		move_0100(c);
	} else {
		move_rand(c, 0);
	}
	return 0;
}

/* unintelligent, telepathic, tunneling, non-erratic */
int move_0110(Character *c)
{
	// move directly to pc with tunneling
		int dx, dy;

		//NW
		if(pc->x - c->x < 0 && pc->y - c->y < 0) {
			dx = -1;
			dy = -1;
		} // N 
		else if (pc->x - c->x == 0 && pc->y - c->y < 0) {
			dx = 0;
			dy = -1;
		} // NE 
		else if (pc->x - c->x > 0 && pc->y - c->y < 0) {
			dx = 1;
			dy = -1;
		} // E 
		else if (pc->x - c->x > 0 && pc->y - c->y == 0) {
			dx = 1;
			dy = 0;
		} // SE 
		else if (pc->x - c->x > 0 && pc->y - c->y > 0) {
			dx = 1;
			dy = 1;
		}  // S 
		else if (pc->x - c->x == 0 && pc->y - c->y > 0) {
			dx = 0;
			dy = 1;
		} // SW 
		else if (pc->x - c->x < 0 && pc->y - c->y > 0) {
			dx = -1;
			dy = 1;
		} // W 
		else if (pc->x - c->x < 0 && pc->y - c->y == 0) {
			dx = -1;
			dy = 0;
		}

		if (tmap[c->y + dy][c->x + dx] == ROCK) {
			tunnel(c, c->x + dx, c->y + dy);
		} else {
			character_move(c, c->x + dx, c->y + dy);
		}

		return 0;
}

/* unintelligent, telepathic, tunneling, erratic */
int move_0111(Character *c)
{
	if (rand() % 2) {
		move_0110(c);
	} else {
		move_rand(c, 1);
	}
	return 0;
}

/* intelligent, non-telepathic, non-tunneling, non-erratic */
int move_1000(Character *c)
{
	int dx, dy, i, j;
	int min = 255;
	for(i = -1; i <= 1; i++) {
		for(j = -1; j <= 1; j++) {
			if(distN[c->y + i][c->x + j] < min){
				min = distN[c->y + i][c->x + j];
				dy = i;
				dx = j;
			}
		}
	}

	character_move(c, c->x + dx, c->y + dy);
	return 0;
}

/* intelligent, non-telepathic, non-tunneling, erratic */
int move_1001(Character *c)
{
	if (rand() % 2) {
		move_1000(c);
	} else {
		move_rand(c, 0);
	}
	return 0;
}

/* intelligent, non-telepathic, tunneling, non-erratic */
int move_1010(Character *c)
{

	return 0;
}

/* intelligent, non-telepathic, tunneling, erratic */
int move_1011(Character *c)
{

	return 0;
}

/* intelligent, telepathic, non-tunneling, non-erratic */
int move_1100(Character *c)
{

	return 0;
}

/* intelligent, telepathic, non-tunneling, erratic */
int move_1101(Character *c)
{

	return 0;
}

/* intelligent, telepathic, tunneling, non-erratic */
int move_1110(Character *c)
{

	return 0;
}

/* intelligent, telepathic, tunneling, erratic */
int move_1111(Character *c)
{

	return 0;
}



int move_monster(Character *c)
{
	switch(c->c)
	{
	     case '0': move_0000(c); break;
	     case '1': move_0001(c); break;
	     case '2': move_0010(c); break;
	     case '3': move_0011(c); break;
	     case '4': move_0100(c); break;
	     case '5': move_0101(c); break;
	     case '6': move_0110(c); break;
	     case '7': move_0111(c); break;
	     case '8': move_1000(c); break;
	     case '9': move_1001(c); break;
	     case 'a': move_1010(c); break;
	     case 'b': move_1011(c); break;
	     case 'c': move_1100(c); break;
	     case 'd': move_1101(c); break;
	     case 'e': move_1110(c); break;
	     case 'f': move_1111(c); break;
	     default: return 1;
     }


	return 0;
}

int move_pc(Character *c)
{
	int dx, dy;
	do {
			dx = rand() % 3 - 1;
			dy = rand() % 3 - 1;
		} while (hmap[c->y + dy][c->x + dx] == IMMUTABLE);
		character_move(c, c->x + dx, c->y + dy);
		/* updates map if char tunnels through rock */
		if (hmap[c->y][c->x] > 0) {
			hmap[c->y][c->x] = 0;
			tmap[c->y][c->x] = CORR;
			dijkstra_init(pc->x, pc->y, 1);
		}

	/* Update distance maps */
	dijkstra_init(pc->x, pc->y, 0);
	
	return 0;
}
#include <stdio.h>
#include <stdlib.h>

#include "character.h"
#include "debug.h"
#include "dijkstra.h"
#include "dungeon.h"
#include "move.h"
#include "point.h"

/* scan count:= number of monsters found in the scanned area */
int scc = 0;

/* scan array:= monsters found in the scanned area */
Character *scv[3*3];

/* scan the area centered at (x, y) then store the result in scv */
static int scanArea(int x, int y, int radius)
{
	int r, c;
	scc = 0;

	for (r=y-radius; r<=y+radius; r++) {
		for (c=x-radius; c<=x+radius; c++) {
			if (c==pc->x && r==pc->y)
				continue;
			if (r<=0 || r>=DUNG_H - 1)
				continue;
			if (c<=0 || c>=DUNG_W - 1)
				continue;
			if (cmap[r][c])
				scv[scc++] = cmap[r][c];
		}
	}
	return scc;
}

/* move c to the new location (x, y) */
static void update(Character *c, int x, int y)
{
	if (hmap[y][x]) {
		if (hmap[y][x] > 85) {
			// failed to tunnel
			hmap[y][x] -= 85;
			return;
		} else {
			// tunnel successfully
			hmap[y][x] = 0;
			tmap[y][x] = '#';
		}
	}
	// clear old location
	cmap[c->y][c->x] = NULL;

	// go to new location
	c->x = x;
	c->y = y;

	// co:= original character occupied at new location
	Character *co = cmap[c->y][c->x];
	if (co) {
		if (ISPC(co) && cheat) {
			c->dead = 1; // kill whoever attempted to kill PC
			return;
		} else {
			co->dead = 1;
		}
	}
	cmap[c->y][c->x] = c;
}

/* escape algorithm for PC */
static void escape()
{
	// look for optimal escape	
	int x, y, escx = 0, escy = 0, min = 1<<8;
	for (x = pc->x-1; x <= pc->x+1; x++) {
		for (y = pc->y-1; y <= pc->y+1; y++) {
			if (x == pc->x && y == pc->y)
				continue; // skip myself
			if (hmap[y][x])
				continue; // PC cannot tunnel
			if (scanArea(x, y, 1))
				continue; // absolutely not safe

			scanArea(x, y, 2);
			if (scc < min) {
				min = scc;
				escx = x;
				escy = y;
			}
		}
	}
	if (escx && escy)
		update(pc, escx, escy);
	else
		move_random(pc, 0);
}

int move_npc(Character *c)
{	
	int intelligent = (1 & c->c);
	int telepathy = (2 & c->c);
	int tunneling = (4 & c->c);
	int erratic = (8 & c->c);

	if ((erratic && (rand() & 0x1))) {
		move_random(c, tunneling);
	} else if (intelligent && telepathy) {
		move_dijkstra(c, tunneling);
	} else if (intelligent) {
		if (dungeon_sight(c->x, c->y, pc->x, pc->y)) {
			// can see PC
			move_dijkstra(c, tunneling);
			// memorize location of PC
			c->mempcx = pc->x;
			c->mempcy = pc->y;
		} else if (c->mempcx && c->mempcy) {
			// cannot see PC so use memorized PC location
			move_toward(c, c->mempcx, c->mempcy, tunneling);
		} else {
			move_random(c, tunneling);
		}
	} else if (telepathy) {
		move_toward(c, pc->x, pc->y, tunneling); 
	} else {
		move_random(c, tunneling);
	}
	return 0;
}

int move_pc()
{
	if (nummon <= 0) {
		return 0; // game over
	}
	if (cheat) {
		dijkstra_init(npcs[0]->x, npcs[0]->y, 0);
		move_dijkstra(pc, 0);
	} else {
		scanArea(pc->x, pc->y, 3);

		// look for monsters if no monsters are around
		if (scc == 0) {
			dijkstra_init(npcs[0]->x, npcs[0]->y, 0);
			move_dijkstra(pc, 0);
		} else if (scc == 1) {
			// now start fighting the monster
			Character *m = scv[0];
			int tpm = 100/m->speed; // turns per move
			int mvs = 0; // number of moves my enemy will take to reach me
			int mturn = m->turn;
			for (;pc->turn - 10 <= mturn 
					&& mturn < pc->turn; mturn+=tpm, mvs++);	
			int x, y, optx = 0, opty = 0, d = 0;
			for (x = pc->x - 1; x <= pc->x + 1; x++) {
				for (y = pc->y - 1; y <= pc->y + 1; y++) {
					if (x == pc->x && y == pc->y)
						continue;
					if (hmap[y][x])
						continue;

					int dx = ABS((x-m->x));
					int dy = ABS((y-m->y));
					d = MAX(dx, dy); 

					// if I can safely attack or my enemy cannot reach me
					if (d == 0 || d - mvs == 1) {
						optx = x;
						opty = y;

						if (d == 0) // attack near enemy
							break;
					}
				}
				if (d == 0) // attack near enemy
					break;
			}
			if (optx && opty) {
				update(pc, optx, opty); // an optimal move is found
			} else {
				escape(); // no optimal move found
			}
		} else {
			// escape if there are too many monsters around
			escape();
		}
	}

	// calculate distance map for intelligent monsters to use
	dijkstra_init(pc->x, pc->y, 0);
	dijkstra_init(pc->x, pc->y, 1);

	return 0;
}

int move_c(Character *c, int x, int y)
{
	// bad move
	if (x <= 0 || x >= DUNG_W - 1)
		return 1;
	if (y <= 0 || y >= DUNG_H - 1)
		return 1;
	if (!(c->c & 0x8) && hmap[y][x]) // cannot tunnel
		return 1;

	update(c, x, y);

	if (ISPC(c)) {
		// calculate distance map for intelligent monsters to use
		dijkstra_init(pc->x, pc->y, 0);
		dijkstra_init(pc->x, pc->y, 1);
	}
	return 0;
}

int move_dijkstra(Character *c, int t)
{
	Point p = dijkstra_next(c->x, c->y, t);

	update(c, p.x, p.y);

	return 0;
}

int move_random(Character *c, int t)
{	
	int x, y;

	while (1) {
		x = c->x;
		y = c->y;

		switch (rand() & 0x7) {
			case 0: x++; break;
			case 1: x--; break;
			case 2: y++; break;
			case 3: y--; break;
			case 4: x++; y++; break;
			case 5: x--; y++; break;
			case 6: x++; y--; break;
			case 7: x--; y--; break;
		}

		// either tunneling or find a open space
		if ((t && hmap[y][x]!=IMMUTABLE) || (!hmap[y][x]))
			break;
	}
	update(c, x, y);

	return 0;
}

int move_toward(Character *c, int x, int y, int t)
{
	int dx = x - c->x;
	int dy = y - c->y;

	int incx = 0;
	int incy = 0;
	if (dx > 0) incx =  1;
	if (dx < 0) incx = -1;
	if (dy > 0) incy =  1;
	if (dy < 0) incy = -1;

	int nextx = c->x + incx;
	int nexty = c->y + incy;

	if (t || !hmap[nexty][nextx])
		update(c, nextx, nexty);
	else
		move_random(c, t); // get stuck

	return 0;
}

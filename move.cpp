#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#include <vector>

#include "character.h"
#include "debug.h"
#include "dijkstra.h"
#include "dungeon.h"
#include "move.h"
#include "npc.h"
#include "pc.h"
#include "point.h"
#include "ui.h"

/* scan count:= number of monsters found in the scanned area */
int scc = 0;

/* scan array:= monsters found in the scanned area */
Character *scv[BUFFER_SIZE];

static int dist(int x1, int y1, int x2, int y2)
{
	int dx = ABS((x1-x2));
	int dy = ABS((y1-y2));
	return MAX(dx, dy); 
}

/* scan the area centered at (x, y) then store the result in scv */
static int scanArea(int x, int y, int radius)
{
	scc = 0;

	for (int r=y-radius; r<=y+radius; r++) {
		for (int c=x-radius; c<=x+radius; c++) {
			int pcx, pcy;
			pc->getLocation(&pcx, &pcy);

			if (c==pcx && r==pcy)
				continue;
			if (r<=0 || r>=DUNG_H - 1)
				continue;
			if (c<=0 || c>=DUNG_W - 1)
				continue;
			if (dungeon->cmap[r][c])
				scv[scc++] = dungeon->cmap[r][c];
		}
	}
	return scc;
}

// move c to the new location (x, y)
static void update(Character *c, int x, int y)
{
	if (dungeon->hmap[y][x]) {
		if (dungeon->hmap[y][x] > 85) {
			// failed to tunnel
			dungeon->hmap[y][x] -= 85;
			return;
		} else {
			// tunnel successfully
			dungeon->hmap[y][x] = 0;
			dungeon->tmap[y][x] = '#';
		}
	}
	
	int cx, cy;
	c->getLocation(&cx, &cy);
	
	// clear old location
	dungeon->cmap[cy][cx] = NULL;
	
	// go to new location
	c->setLocation(x, y);
	
	// co:= original character occupied at new location
	Character *co = dungeon->cmap[y][x];
	if (co) {
		if (co->isPC() && invulnerable) {
			c->setDead(); // kill whoever attempted to kill PC
			return;
		} else {
			co->setDead();
		}
	}
	
	dungeon->cmap[y][x] = c;
}

// escape algorithm for PC
static void escape()
{
	// look for optimal escape	
	int x, y, escx = 0, escy = 0, min = 1<<8;
	int pcx, pcy;
	pc->getLocation(&pcx, &pcy);
	
	for (x = pcx-1; x <= pcx+1; x++) {
		for (y = pcy-1; y <= pcy+1; y++) {
			if (x == pcx && y == pcy)
				continue; // skip myself
			if (dungeon->hmap[y][x])
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
		Move::random(pc, 0);
}

int Move::npc(NPC *c)
{	
	int abil = c->getAbil();
	int cx, cy, mempcx, mempcy;

	c->getLocation(&cx, &cy);

	c->getMemPCLocation(&mempcx, &mempcy);

	// intelligence
	int smart = (1 & abil);
	// telepathy
	int tele = (2 & abil);
	// tunneling
	int tunnel = (4 & abil);
	// erratic
	int erratic = (8 & abil);

	// erractic desicion or no moving instruction
	if ((erratic && rand() % 2) || (!smart && !tele)) {
		Move::random(c, tunnel); // 4 & c->c gives tunneling
	} else if (smart && tele) {
		Move::dijkstra(c, tunnel);
	} else if (smart) {
		if (dungeon->isVisible(cx, cy, pcx, pcy)) {
			Move::dijkstra(c, tunnel);
			c->setMemPCLocation(pcx, pcy);
		} else if (mempcx && mempcy) {
			Move::toward(c, mempcx, mempcy, tunnel);
		} else {
			Move::random(c, tunnel);
		}
	} else if (tele) {
		Move::toward(c, pcx, pcy, tunnel); 
	} else {
		Move::random(c, tunnel);
	}
	return 0;
}

int Move::pcAI()
{
	if (dungeon->nummon() <= 0) {
		return 0; // game over
	}
	
	int npcx, npcy;
	dungeon->npcv[0]->getLocation(&npcx, &npcy);
	
	if (invulnerable) {
		Dijkstra::run(npcx, npcy, 0);
		Move::dijkstra(pc, 0);
	} else {
		// PC move algorithm
		scanArea(pcx, pcy, 3);
 
		if (scc == 0) {
			// look for monsters if no monsters are around
			Dijkstra::run(npcx, npcy, 0);
			Move::dijkstra(pc, 0);
		} else if (scc == 1) {
			// fight the monster if there is only one
			Character *m = scv[0];
			int mx, my;
			m->getLocation(&mx, &my);

			int speed = m->getSpeed();
			int tpm = 100/speed; // turns per move
			int mvs = 0; // number of moves my enemy will take to reach me
			int mturn = m->getTurn();
			int pcturn = pc->getTurn();
			for (;pcturn - 100/pc->getSpeed() <= mturn 
					&& mturn < pcturn; mturn+=tpm, mvs++);	
			int x, y, optx = 0, opty = 0, d = 1<<10;
			for (x = pcx - 1; x <= pcx + 1; x++) {
				for (y = pcy - 1; y <= pcy + 1; y++) {
					if (x == pcx && y == pcy)
						continue;
					if (dungeon->hmap[y][x])
						continue;
					d = dist(x, y, mx, my);

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
				update(pc, optx, opty);
			} else {
				escape();
			}
		} else {
			// escape if there are too many monsters around
			escape();
		}
	}

	// calculate distance map for intelligent monsters to use
	Dijkstra::run(pcx, pcy, 0);
	Dijkstra::run(pcx, pcy, 1);

	return 0;
}

int Move::move(Character *c, int x, int y)
{
	// bad move
	if (x <= 0 || x >= DUNG_W - 1)
		return 1;
	if (y <= 0 || y >= DUNG_H - 1)
		return 1;
	if (dungeon->hmap[y][x])
		return 1;

	Debug::log("move %c to (%d, %d)\n",
	c->getSymb(), x, y);
			
	update(c, x, y);

	if (c->isPC()) {
		int pcx, pcy;
		pc->getLocation(&pcx, &pcy);

		// calculate distance map for intelligent monsters to use
		Dijkstra::run(pcx, pcy, 0);
		Dijkstra::run(pcx, pcy, 1);
	}
	return 0;
}

int Move::dijkstra(Character *c, int t)
{
	Point next = Dijkstra::next(c->getX(), c->getY(), t);
	
	update(c, next.x, next.y);

	return 0;
}

int Move::random(Character *c, int t)
{	
	int x, y;
	
	while (1) {
		int r = rand() % 8;

		c->getLocation(&x, &y);

		switch (r) {
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
		if ((t && dungeon->hmap[y][x]!=IMMUTABLE) || 
				(!dungeon->hmap[y][x])) {
			break;
		} else {
			r = rand() % 8;	
		}
	}

	update(c, x, y);
		
	return 0;
}

int Move::toward(Character *c, int x, int y, int t)
{
	int cx, cy;
	c->getLocation(&cx, &cy);

	int dx = x - cx;
	int dy = y - cy;

	int incx = 0;
	int incy = 0;
	if (dx > 0) incx =  1;
	if (dx < 0) incx = -1;
	if (dy > 0) incy =  1;
	if (dy < 0) incy = -1;
	
	int nextx = cx + incx;
	int nexty = cy + incy;
	
	if (t || !dungeon->hmap[nexty][nextx])
		update(c, nextx, nexty);
	else
		Move::random(c, t); // get stuck
	
	return 0;
}

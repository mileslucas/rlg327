#ifndef MOVE_H
#define MOVE_H

#include "core.h"

/* automatically move NPC */
int move_npc(Character*);

/* automatically move PC (AI) */
int move_pc();

/* manually move character */
int move_c(Character*, int, int);

/* choose next move based on distN or distT */
int move_dijkstra(Character*, int tunneling);

/* choose next move randomly */
int move_random(Character*, int tunneling);

/* choose next move based on direction of (x, y) */
int move_toward(Character*, int x, int y, int tunneling);

#endif


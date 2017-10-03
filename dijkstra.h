#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "core.h"
#include "point.h"

int distN[DUNG_H][DUNG_W]; // non-tunneling
int distT[DUNG_H][DUNG_W]; // tunneling

Point dijkstra_next(int x, int y, int tunneling);

int dijkstra_print(int tunneling);

int dijkstra_init(int x, int y, int tunneling);

#endif


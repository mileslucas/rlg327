#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "core.h"
#include "point.h"

/* distance map */
int distN[DUNG_H][DUNG_W]; // non-tunneling
int distT[DUNG_H][DUNG_W]; // tunneling

/* 1.03 print distance map */
int dijkstra_print(int tunneling);

/* 1.03 initialize distance map */
int dijkstra_init(int x, int y, int tunneling);

#endif


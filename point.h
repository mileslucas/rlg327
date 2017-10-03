#ifndef POINT_H
#define POINT_H

typedef struct
{
	unsigned char x, y;
} Point;

Point point(int x, int y);

void point_neighbors(Point neighbors[8], int x, int y);

#endif


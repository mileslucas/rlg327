#include <stdlib.h>

#include "point.h"

Point point(int x, int y)
{
	Point p = {x, y};
	return p;
}

void point_neighbors(Point ns[8], int x, int y)
{
	ns[0] = point(x, y - 1); // N
	ns[1] = point(x, y + 1); // S
	ns[2] = point(x - 1, y); // W
	ns[3] = point(x + 1, y); // E
	ns[4] = point(x - 1, y - 1); // NW
	ns[5] = point(x + 1, y - 1); // NE
	ns[6] = point(x - 1, y + 1); // SW
	ns[7] = point(x + 1, y + 1); // SE
}


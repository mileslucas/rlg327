#include "core.h"
#include "dungeon.h"
#include "room.h"

static int numAdjacentCorridors(int r, int c)
{
	int num = 0;
	if(tmap[r-1][c]==CORR)
		num++;
	if(tmap[r+1][c]==CORR)
		num++;
	if(tmap[r][c-1]==CORR)
		num++;
	if(tmap[r][c+1]==CORR)
		num++;
	return num;
}

void corridor_connect(Room from, Room to)
{
	Point f = room_centroid(from);
	Point t = room_centroid(to);

	int dx = t.x - f.x;
	int dy = t.y - f.y;

	int incx = dx > 0 ? 1 : -1;
	int incy = dy > 0 ? 1 : -1;

	int r = f.y, c = f.x;

	while (r!=t.y && c!=t.x) {	
		if (tmap[r][c] != ROOM) {
			tmap[r][c] = CORR;
			hmap[r][c] = 0;
		}
		// if we connect to existing path, then stop
		if (numAdjacentCorridors(r, c)>1)
			break;

		// prefer less hard path
		if (hmap[r][c+incx]<hmap[r+incy][c])
			c+=incx;
		else
			r+=incy;
	}
	while (r != t.y || c != t.x) {
		if (tmap[r][c] != ROOM) {
			tmap[r][c] = CORR;
			hmap[r][c] = 0;
		}
		// if we connect to existing path, then stop
		if (numAdjacentCorridors(r, c)>1)
			break;

		if (r!=t.y)
			r+=incy;
		if (c!=t.x)
			c+=incx;
	}
}


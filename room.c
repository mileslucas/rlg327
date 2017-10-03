#include <stdlib.h>
#include <time.h>

#include "core.h"
#include "dungeon.h"
#include "room.h"

Point room_centroid(Room r)
{
	return point(r.x + r.w/2, r.y + r.h/2);
}

int room_collide(Room r, Room s)
{
	if(r.x+r.w < s.x || r.x > s.x+s.w)
		return 0;
	if(r.y+r.h < s.y || r.y > s.y+s.h)
		return 0;

	return 1;
}

Room room_create()
{
	Room r;
	r.w = rand() % (ROOM_MAX_W - ROOM_MIN_W) + ROOM_MIN_W;
	r.h = rand() % (ROOM_MAX_H - ROOM_MIN_H) + ROOM_MIN_H;
	r.x = rand() % (DUNG_W - r.w - 1) + 1;
	r.y = rand() % (DUNG_H - r.h - 1) + 1;
	return r;
}

void room_place(Room room)
{
	int r, c;
	for(r=room.y;r<room.y+room.h;r++)
	{
		for(c=room.x;c<room.x+room.w;c++)
		{
			tmap[r][c] = ROOM;
			hmap[r][c] = 0;
		}
	}
}


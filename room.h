#ifndef ROOM_H
#define ROOM_H

#include "point.h"

typedef struct
{
	unsigned char x, y, w, h;
} Room;

/* all the rooms that have been added to the dungeon */
Room rooms[ROOM_MAX_COUNT];

/* number of rooms */
int roomc;

/* get the centroid of the room */
Point room_centroid(Room);

/* determine if two rooms are adjacent or overlap */
int room_collide(Room, Room);

/* create a new room with random location and size */
Room room_create();

/* place the room on the dungeon */
void room_place(Room);

void room_randomLocation(int *x, int *y, Room r);

#endif


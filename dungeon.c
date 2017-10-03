#include "core.h"
#include "corridor.h"
#include "character.h"
#include "debug.h"
#include "dungeon.h"
#include "room.h"


char *loadp = NULL;
char *savep = NULL;

int sightmap[DUNG_H][DUNG_W]; // for visualization of line of sight

static int initBorder()
{
	int r, c;

	// initialize immutable borders
	for (r=0;r<DUNG_H;r++) {
		tmap[r][0] = ROCK;
		tmap[r][DUNG_W-1] = ROCK;

		hmap[r][0] = IMMUTABLE;
		hmap[r][DUNG_W-1] = IMMUTABLE;
	}
	for (c=0;c<DUNG_W;c++) {
		tmap[0][c] = ROCK;
		tmap[DUNG_H-1][c] = ROCK;

		hmap[0][c] = IMMUTABLE;
		hmap[DUNG_H-1][c] = IMMUTABLE;
	}

	return 0;
}

int dungeon_print()
{
	int r, c, i, n;

	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
	init_pair(4, COLOR_MAGENTA, COLOR_BLACK);

	// Print leading line
	mvprintw(0, 0, "Number of Monsters Remaining: %2d", nummon);
	mvprintw(0, 69, "Miles Lucas\n");

	// line of sight visualization
	if (sight) {	
		for (r=0; r<DUNG_H; r++) {
			for (c=0; c<DUNG_W; c++) {
				// clear old sight map
				sightmap[r][c] = 0;
			}
		}
		for (i=0; i<nummon; i++) {
			if (!(0x2 & npcs[i]->c)) {
				// display line of sight only for non-telepathic monsters
				dungeon_sight(npcs[i]->x, npcs[i]->y, pc->x, pc->y);
			}
		}
	}

	// print dungeon
	for (r=0; r<DUNG_H; r++) {
		for (c=0; c<DUNG_W; c++) {
			if (cmap[r][c]) {
				int speed = cmap[r][c]->speed;
				int ch = character_char(cmap[r][c]);

				if (ISPC(cmap[r][c])){ // if it is PC
					n = 1;
				}else if (speed <= 12){
					n = 2;
				}else if (speed <= 16){
					n = 3;
				}else if (speed <= 20){
					n = 4;
				}
				attron(COLOR_PAIR(n));
					addch(ch);
					attroff(COLOR_PAIR(n));
				
			} else if (sight && sightmap[r][c]) {
				// light of sight visualization
				if (sightmap[r][c] > 0) {
					attron(COLOR_PAIR(2));
					addch('o');
					attroff(COLOR_PAIR(2));;
				} else {
					attron(COLOR_PAIR(1));
					addch('x');
					attroff(COLOR_PAIR(1));
				}
				
			} else {
				// print dungeon cell on the dungeon
				addch(tmap[r][c]);
			}
		}
	}
	// print trailing two lines
	mvprintw(22, 0, "A night to remember...\n(m)onsters List \t (Q)uit");
	// Refresh screen
	refresh();
	return 0;
}

/* 1.04 line of sight */
int dungeon_sight(int x1, int y1, int x2, int y2)
{
	int dx = x2 - x1;
	int dy = y2 - y1;

	int incx = 0;
	int incy = 0;

	if (dx > 0) incx =  1;
	if (dx < 0) incx = -1;
	if (dy > 0) incy =  1;
	if (dy < 0) incy = -1;

	int curx = x1;
	int cury = y1;

	int blocked = 0;

	while (curx!=x2 || cury!=y2) {
		if (hmap[cury][curx])
			blocked = 1;

		if (blocked)
			sightmap[cury][curx] = -1; // -1 means blocked
		else
			sightmap[cury][curx] =  1; //  1 means not blocked

		if (ABS(dx)>ABS(dy)) {
			curx += incx;
			cury = y1 + (curx - x1) * dy / dx;
		} else {
			cury += incy;
			curx = x1 + (cury - y1) * dx / dy;
		}
	}
	return !blocked;
}

/* 1.02 --load */
int dungeon_load()
{
	int r, c, i;

	FILE *fp = fopen(loadp, "rb");
	if(!fp)
	{
		fprintf(stderr, "Could not load file: %s not exist\n", loadp);
		return 1;
	}

	// 0-5 RLG327
	char marker[7];
	fread(marker, sizeof(*marker), 6, fp);
	marker[6] = '\0';
	if (strcmp(marker, "RLG327"))
	{
		fprintf(stderr, "Could not load file: %s: not a RLG327\n", loadp);
		fclose(fp);
		return 1;
	}

	// 6-13
	unsigned int buffer[2];
	fread(buffer, sizeof(buffer), 1, fp);
	roomc = (be32toh(buffer[1]) - 1694)/4;

	// 14-1693
	fread(hmap, sizeof(hmap), 1, fp);
	for (r=0; r<DUNG_H; r++)
	{
		for (c=0; c<DUNG_W; c++)
		{
			if (!hmap[r][c])
				tmap[r][c] = CORR;
			else
				tmap[r][c] = ROCK;
		}
	}

	// 1694-end
	for (i=0; i<roomc; i++)
	{
		fread(&rooms[i].x, 1, 1, fp);
		fread(&rooms[i].w, 1, 1, fp);
		fread(&rooms[i].y, 1, 1, fp);
		fread(&rooms[i].h, 1, 1, fp);

		room_place(rooms[i]);	
	}

	fclose(fp);

	return 0;
}

/* 1.02 --save */
int dungeon_save()
{
	int i;

	FILE *fp = fopen(savep, "wb");
	if(!fp)
	{
		fprintf(stderr, "Could not save file: %s not exist\n", savep);
		return 1;
	}

	// 0-5
	fwrite("RLG327", 6, 1, fp);

	// 6-13
	unsigned int buffer[2];
	buffer[0] = 0;
	buffer[1] = htobe32(roomc * 4 + 1694);
	fwrite(buffer, sizeof(buffer), 1, fp);

	// 14-1693
	fwrite(hmap, sizeof(hmap), 1, fp);

	// 1694-end
	for (i=0; i<roomc; i++)
	{
		fwrite(&rooms[i].x, 1, 1, fp);
		fwrite(&rooms[i].w, 1, 1, fp);
		fwrite(&rooms[i].y, 1, 1, fp);
		fwrite(&rooms[i].h, 1, 1, fp);
	}

	fclose(fp);

	return 0;
}



/* 1.01 generate random dungeon */
int dungeon_generate()
{
	int i, t, tmax = (1<<5); // max number of attempts to create rooms
	int r, c;

	initBorder();

	// fill the tmap with ROCK
	for(r=1;r<DUNG_H-1;r++)
	{
		for(c=1;c<DUNG_W-1;c++)
		{
			tmap[r][c] = ROCK;
			hmap[r][c] = rand() % 254 + 1;
		}
	}

	for (i=0, t=0; t<tmax; t++)
	{
		rooms[i] = room_create();

		// determine if the new room collides with any previous room	
		int hasCollision = 0;
		int j;
		for (j=0;j<i;j++)
		{
			if(room_collide(rooms[i], rooms[j]))
			{
				hasCollision = 1;
				break;
			}
		}

		if (!hasCollision)
		{
			room_place(rooms[i]);

			corridor_connect(rooms[i], rooms[0]);

			i++;
		}
	}
	roomc = i;

	return 0;
}


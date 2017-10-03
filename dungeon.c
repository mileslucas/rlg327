#include "core.h"
#include "corridor.h"
#include "character.h"
#include "debug.h"
#include "dungeon.h"
#include "room.h"

char *loadp = NULL;
char *savep = NULL;

static int initBorder()
{
	int r, c;

	// initialize immutable borders
	for (r=0;r<DUNG_H;r++)
	{
		tmap[r][0] = ROCK;
		tmap[r][DUNG_W-1] = ROCK;
			
		hmap[r][0] = IMMUTABLE;
		hmap[r][DUNG_W-1] = IMMUTABLE;
	}
	for (c=0;c<DUNG_W;c++)
	{
		tmap[0][c] = ROCK;
		tmap[DUNG_H-1][c] = ROCK;
		
		hmap[0][c] = IMMUTABLE;
		hmap[DUNG_H-1][c] = IMMUTABLE;
	}

	return 0;
}

int dungeon_print()
{
	int r, c;
	
	for (r=0;r<DUNG_H;r++)
	{
		for (c=0;c<DUNG_W;c++)
		{
			if (cmap[r][c])
			{
				// print character on the tmap
				printf("%c", cmap[r][c]->c);
			}
			else
			{
				// print dungeon cell on the dungeon
				printf("%c", tmap[r][c]);
			}
		}
		printf("\n");
	}
	
	return 0;
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


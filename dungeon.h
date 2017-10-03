#ifndef DUNGEON_H
#define DUNGEON_H

/* load path */
extern char* loadp;
/* save path */
extern char* savep;

/* terrain */
char tmap[DUNG_H][DUNG_W];

/* hardness */
unsigned char hmap[DUNG_H][DUNG_W];

/* 1 if display line of sight */
extern int sight;

int dungeon_clear();

int dungeon_print();

/* 1.04 returns 1 if there is a line of sight between two points */
int dungeon_sight(int, int, int, int);

/* 1.02 --load */
int dungeon_load();

/* 1.02 --save */
int dungeon_save();

/* 1.01 generate random dungeon */
int dungeon_generate();

#endif


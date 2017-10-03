#ifndef DUNGEON_H
#define DUNGEON_H

extern char* loadp; /* load path */
extern char* savep; /* save path */

/* terrain */
char tmap[DUNG_H][DUNG_W];

/* hardness */
unsigned char hmap[DUNG_H][DUNG_W];

/* print dungeon */
int dungeon_print();

/* 1.02 load dungeon using --load <file> */
int dungeon_load();

/* 1.02 save dungeon using --save <file> */
int dungeon_save();

/* 1.01 generate random dungeon */
int dungeon_generate();

#endif


#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>

#include "core.h"
#include "character.h"
#include "debug.h"
#include "dungeon.h"
#include "move.h"
#include "npc.h"
#include "pc.h"
#include "ui.h"

static void getPositionStr(char buf[50], Character *c)
{
	int i;
	for(i=0; i<50; i++) buf[i]=' ';

	int cx, cy, pcx, pcy;
	c->getLocation(&cx, &cy);
	pc->getLocation(&pcx, &pcy);

	int dx = cx - pcx;
	int dy = cy - pcy;
	
	if (dx>0)
		sprintf(buf + 2, "%02d E             ",  dx);
	else
		sprintf(buf + 2, "%02d W             ", -dx);
	if (dx==0)
		sprintf(buf + 2, "00         ");
	if (dy>0)
		sprintf(buf + 8,          "%02d S    ",  dy);
	else
		sprintf(buf + 8,          "%02d N    ", -dy);
	if (dy==0)
		sprintf(buf + 8, "00         ");
}

static int compareNPC(const void *a, const void *b)
{
	int ax, ay, bx, by, pcx, pcy;

	(*(Character **)a)->getLocation(&ax, &ay);
	(*(Character **)b)->getLocation(&bx, &by);
	pc->getLocation(&pcx, &pcy);
	
	return (ABS((ax-pcx))+ABS((ay-pcy)))
	     - (ABS((bx-pcx))+ABS((by-pcy)));
}

int UI::help()
{
	FILE* fp = fopen("README", "r");

	if (!fp) {
		fprintf(stderr, "README not found\n");
		return -1;
	}

	// ncurses stuff
	initscr();
	start_color();
	raw();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);

	int r = 0, c = 0;
	char buffer[255][80];
	while (fgets(buffer[r++], 80, fp));
	fclose(fp);

	int rmax = r;

	int from = 0;

	while (1) {
		clear();

		char tmp[80];
		char *p;

		for (r=0; r<22; r++) {
			p = tmp;

			for (c=0; c<80; c++) {
				if ('\t'==buffer[from+r][c]) {
					sprintf(p, "    ");
					p+=4;
				}
				else *p++ = buffer[from+r][c];
			}
			mvprintw(r, 0, "%s", tmp);
		}

		int color;
		init_pair(color = 9, COLOR_GREEN, COLOR_BLACK);

		attron(COLOR_PAIR(color));

		mvprintw(24-1, 0, 
				"Use arrow keys to scroll up and down. Press Q to quit.");

		attroff(COLOR_PAIR(color));

		refresh();

		int ch = getch();
		if (ch=='Q' || ch=='q') break;
		if (ch==KEY_DOWN) 
			if (from+22<rmax) 
				from++;
		if (ch==KEY_UP)   
			if (from>0)
				from--;
	}
	endwin();

	return 0;
}

int UI::initColors()
{
	init_pair(COLOR_PC, COLOR_RED, COLOR_YELLOW);
	
	for (int i = 0; i < 8; i++) {
		init_pair(i, i, COLOR_BLACK);
	}

	return 0;
}

static void mListFrom(int start)
{
	int i;
	char buf[50];

	int nummon = dungeon->nummon();

	mvprintw(0, 1, "Listing Monster %02d to %02d out of %02d "
	"(press ESC or Q to quit)", 
	start+1, MIN((start+21), nummon), nummon);

	std::sort(dungeon->npcv.begin(), dungeon->npcv.end(), compareNPC);

	for (i=start; i<MIN((start+21), nummon); i++) {
		Character *c = dungeon->npcv[i];
		getPositionStr(buf, c);
		
		mvprintw(i-start+1, 2, "  %s", buf);
	
		mvaddch(i-start+1, 18, ACS_VLINE);

		int color = c->getColor();
		
		attron(COLOR_PAIR(color));
		mvprintw(i-start+1, 2, "%c", c->getSymb());
		attroff(COLOR_PAIR(color));

	}
	int r = MIN((start+21), nummon)-start+1;
	for (int c=1; c<18; c++) {
		mvaddch(r, c, ACS_HLINE);
	}
	mvaddch(r, 18, ACS_LRCORNER);
}

int UI::clearRow(int row)
{
	int i;
	for(i=0; i<80; i++)
		mvaddch(row, i, ' ');
	
	return 0;
}

int UI::reprint()
{
	dungeon->printDungeon();
	return 0;
}

int UI::mList()
{
	int from = 0;

	mListFrom(from);
	refresh();

	while (1) {
		int ch = getch();
		
		int quit = 0;
		
		switch (ch) {
			case 'Q':
			case 'q':
			case 27: // ESC
				quit = 1;
				break;
			case KEY_DOWN:
				if (from>21-nummon && from<nummon-21) from++;
				mListFrom(from);
				refresh();
				break;
			case KEY_UP:
				from = MAX((from-1), 0);
				mListFrom(from);
				refresh();
				break;
		}
		if (quit)
			break;
	}
	
	for (int i=0; i<80; i++) {
		mvprintw(0,  i, " ");
		mvprintw(22, i, " ");
	}

	return 0;
}


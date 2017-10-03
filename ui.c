#include "character.h"
#include "core.h"
#include "debug.h"
#include "ui.h"

static int cmp(const void *av, const void *bv) {
	Character *a = *(Character **)av;
	Character *b = *(Character **)bv;

	return (ABS(a->x-pc->x) + ABS(a->y-pc->y)) - 
	       (ABS(b->x-pc->x) + ABS(b->y-pc->y));
}

static void getPosition(char buf[50], Character *c)
{
	int i;
	for(i=0; i<50; i++) buf[i]=' ';

	int dx = c->x - pc->x;
	int dy = c->y - pc->y;

	if (dx>0)
		sprintf(buf +  5, "%02d east             ",  dx);
	else
		sprintf(buf +  5, "%02d west             ", -dx);
	if (dx==0)
		sprintf(buf +  5, "00         ");
	
	if (dy>0)
		sprintf(buf + 15,          "%02d south  |",  dy);
	else
		sprintf(buf + 15,          "%02d north  |", -dy);
	if (dy==0)
		sprintf(buf + 15, "00        |");
}

static void mListFrom(int start)
{
	int i;
	char buf[50];

	mvprintw(0, 0, "Listing monster %d to %d out of %d "
			"(press ESC or Q to quit)", 
			start+1, MIN((start+21), nummon), nummon);

	for (i=start; i<MIN((start+21), nummon); i++) {
		Character *c = npcs[i];
		getPosition(buf, c);

		mvprintw(i-start+1, 1, "  %s", buf);

		int color = character_color(c);
		attron(COLOR_PAIR(color));
		mvprintw(i-start+1, 1, "%c", character_char(c));
		attroff(COLOR_PAIR(color));
	}
	mvprintw(MIN((start+21), nummon)-start+1, 1, 
			"---------------------------+");
}

int ui_mList()
{
	int from = 0;

	qsort(npcs, nummon, sizeof(void *), cmp);

	mListFrom(from);
	refresh();

	while (1) {
		int ch = getch();

		int quit = 0;

		switch(ch) {
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

	ui_clearRow(0);
	ui_clearRow(22);

	return 0;
}

int ui_initColor()
{
	init_pair(COLOR_PC, COLOR_RED, COLOR_YELLOW);
	// HP
	init_pair(COLOR_HP, COLOR_WHITE, COLOR_RED);
	// MP
	init_pair(COLOR_MP, COLOR_WHITE, COLOR_BLUE);

	for (int i = 0; i < 8; i++)
		init_pair(i,i,COLOR_BLACK);

	return 0;
}

int ui_clearRow(int r)
{
	int c;
	
	for (c=0; c<DUNG_W; c++)
		mvaddch(r, c, ' ');
	
	return 0;
}

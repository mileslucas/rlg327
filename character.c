#include "core.h"
#include "character.h"
#include "dungeon.h"

Character *character_new(int isPC)
{
	Character *c = malloc(sizeof(*c));
	
	if (isPC) {
		c->c = PC_C; // PC characteristic
		c->speed = 10;
	} else {
		c->c = rand() & 0xf;
		c->speed = 5 + (rand() & 0xf); // 5-20 for monsters
	}
	
	c->dead = 0;
	c->turn = 0;

	c->mempcx = 0;
	c->mempcy = 0;

	return c;
}

int character_delete(Character *c)
{
	int i;
	if (!ISPC(c)) {
		// make sure deleted character not in [0, nummon) of npcs
		for (i=0; i<nummon; i++) {
			if (npcs[i]==c) {
				npcs[i] = npcs[--nummon];
			}
		}
	}
	free(c);		
	return 0;
}

char character_char(Character *c)
{
	if (ISPC(c))
		return '@';
	else if (c->c < 10)
		return '0' + c->c;
	else if (c->c < 16)
		return 'a' + c->c - 10;
	else
		return -1;
}

int character_place(Character *c)
{
	while (1) {
		int x = rand() % DUNG_W;
		int y = rand() % DUNG_H;

		if (!hmap[y][x]) {
			cmap[y][x] = c;

			c->x = x;
			c->y = y;
			break;
		}
	}

	if (ISPC(c))
		pc = c;

	return 0;
}

int print_win(WINDOW *win, int start, int end) {
	int i, dx, dy;
	Character *c;
	wattron(win, A_BOLD);
	mvwprintw(win, 0, 0, "Number of known monsters: %d\n\n", nummon);
	wattroff(win, A_BOLD);
	for(i = start; i < MIN(nummon, end); i++) {
		c = npcs[i];
		wprintw(win, "%2d   ", i+1);
		dx = c->x - pc->x;
		dy = c->y - pc->y;

		if (c->speed <= 12){
			wattron(win, COLOR_PAIR(2));
			waddch(win, character_char(c));
			wattroff(win, COLOR_PAIR(2));
		}else if (c->speed <= 16){
			wattron(win, COLOR_PAIR(3));
			waddch(win, character_char(c));
			wattroff(win, COLOR_PAIR(3));
		}else if (c->speed <= 20){
			wattron(win, COLOR_PAIR(4));
			waddch(win, character_char(c));
			wattroff(win, COLOR_PAIR(4));
		}
		if (dx > 0 && dy > 0){
			wprintw(win, ": %2d North by %2d East\n", dy, dx);
		} else if (dx > 0 && dy == 0) {
			wprintw(win, ":             %2d East\n", dx);
		} else if (dx > 0 && dy < 0) {
			wprintw(win, ": %2d South by %2d East\n", -dy, dx);
		} else if (dx == 0 && dy < 0) {
			wprintw(win, ": %2d South\n", -dy);
		} else if (dx < 0 && dy < 0) {
			wprintw(win, ": %2d South by %2d West\n", -dy, -dx);
		} else if (dx < 0 && dy == 0){
			wprintw(win, ":             %2d West\n", -dx);
		} else if (dx < 0 && dy > 0) {
			wprintw(win, ": %2d North by %2d West\n", dy, -dx);
		} else if (dx ==0 && dy > 0) {
			wprintw(win, ": %2d North\n", dy);
		} else {
			return 1;
		}

	}

	mvwprintw(win, 17, 0, "Arrow keys to scroll\nEsc to exit");


	wrefresh(win);
	return 0;
}

int print_characters(void) 
{
	WINDOW *m = newwin(19, 30, 2, 25);
    keypad(m, TRUE);

	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
	init_pair(4, COLOR_MAGENTA, COLOR_BLACK);

	// Clear window
	print_win(m, 0, 14);

	int top = 0;
	int bottom = 14;
	int ch = getch();
	// While 
	while (ch != 27) {
		if (nummon > 14){
			switch(ch) {
				case KEY_UP:
					print_win(m, top==0 ? top : --top, bottom == 14 ? bottom : --bottom);
					break;
				case KEY_DOWN:
					print_win(m, top==nummon - 14 ? top : ++top, bottom == nummon ? bottom : ++bottom);
					break;
			}
		}
		ch = getch();
	}

	return 0;
}


#include "core.h"
#include "corridor.h"
#include "character.h"
#include "debug.h"
#include "dijkstra.h"
#include "dungeon.h"
#include "move.h"
#include "room.h"
#include "turn.h"
#include "ui.h"

int pcx = 0; // 0 means unset
int pcy = 0; // 0 means unset

int cheat = 0;

Character *pc;
Character *npcs[1<<10];

char unify = 0;

unsigned int seed = 0;

int sight = 0;

int nummon = 10;
int nummonmax = 255;

int reset(); // reset everything when PC entering a stair

int main(int argc, char** argv)
{
	char *home = getenv("HOME");

	// make ~/.rlg327 directory
	char dirpath[strlen(home) + strlen("/.rlg327") + 1];
	sprintf(dirpath, "%s/.rlg327", home);
	mkdir(dirpath, S_IRUSR | S_IWUSR | S_IXUSR);

	// ~/.rlg327/dungeon
	char filepath[strlen(home) + strlen("/.rlg327/dungeon") + 1];
	sprintf(filepath, "%s/.rlg327/dungeon", home);

	int i;

	/* options */
	if (argc > 1)
	{
		for (i=1; i<argc; i++)
		{
			if (!strcmp("--debug", argv[i]))
			{
				debug = 1;
			}
			else if (!strcmp("--load", argv[i]))
			{
				if (i+1<argc && argv[i+1][0]!='-')
					loadp = argv[++i];
				else
					loadp = filepath;
			}
			else if (!strcmp("--save", argv[i]))
			{
				if (i+1<argc && argv[i+1][0]!='-')
					savep = argv[++i];
				else
					savep = filepath;
			}
			else if (!strcmp("--nummon", argv[i]))
			{
				if (i+1<argc && argv[i+1][0]!='-')
				{
					nummon = atoi(argv[++i]);

					if (nummon > nummonmax)
					{
						fprintf(stderr, "nummon should not exceed %d\n",
								nummonmax);
						return 1;
					}
				}
				else
				{
					fprintf(stderr, "usage: rlg327 --nummon <number>\n");
					return 1;
				}
			}
			else if (!strcmp("--cheat", argv[i]))
			{
				cheat = 1;
			}
			else if (!strcmp("--pc", argv[i]))
			{
				if (i+2<argc)
				{
					pcx = atoi(argv[++i]);
					pcy = atoi(argv[++i]);
				}
				else
				{
					fprintf(stderr, "usage: rlg327 --pc <x> <y>\n");
					return 1;
				}
			}
			else if (!strcmp("--seed", argv[i]))
			{
				if (i+1<argc)
				{
					seed = atoi(argv[++i]);
				}
				else
				{
					fprintf(stderr, "usage: rlg327 --seed <seed>\n");
					return 1;
				}
			}
			else if (!strcmp("--sight", argv[i]))
			{
				sight = 1;
			}
			else if (!strcmp("--unify", argv[i]))
			{
				if (i+1<argc && argv[i+1][0]!='-')
				{
					unify = argv[++i][0];

					if (!(('0' <= unify && unify <= '9')
								||('a' <= unify && unify <= 'f')))
					{
						fprintf(stderr, "monster must be 0-9 a-f\n");
						return 1;
					}
				}
				else
				{
					fprintf(stderr, "usage: rlg327 --unify <monster>\n");
					return 1;
				}
			}
			// undefined option
			else
			{
				fprintf(stderr, "option %s is undefined.\n", argv[i]);
				fprintf(stderr, "see available options using -h switch.\n");
				return 1;
			}
		}
	}

	if (!seed) 
		// if seed is unset
		srand(seed = time(NULL));
	else 
		// seed is set by -s option
		srand(seed);

	debug_log("debug: seed: %d\n", seed);
	debug_log("debug: save path: %s\n", savep);
	debug_log("debug: load path: %s\n", loadp);

	if (loadp)
		dungeon_load();
	else
		dungeon_generate();

	pc = C_create(1);	

	// init turn heap
	turn_init();
	// insert PC into turn heap
	turn_insert(pc);

	// create monsters and insert them into turn heap
	for (i=0; i<nummon; i++) {
		npcs[i] = C_create(0);
		C_place_char(npcs[i]);
		turn_insert(npcs[i]);
	}
	
	// place PC	
	if (!pcx && !pcy) {
		C_place_char(pc); // randomly place if unset
	} else {
		pc->x = pcx;
		pc->y = pcy;

		cmap[pcy][pcx] = pc;
	}

	// unify all monsters to one type
	if (unify) {
		// unify characteristics
		char unifyc = unify >= 'a' ? unify - 'a' + 10 : unify - '0'; 
		for (i=0; i<nummon; i++)
			npcs[i]->c = unifyc;
	}

	// init distance map
	dijkstra_init(pc->x, pc->y, 0);
	dijkstra_init(pc->x, pc->y, 1);

	// ncurses stuff
	initscr();
	start_color();
	raw();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);

	ui_initColor();

	// game simulation
	while (1) {
		char buffer[100];
		if (pc->dead)
			sprintf(buffer, "YOU LOST ! (press Q to quit)");
		else if (nummon == 0)
			sprintf(buffer, "YOU WON !! (press Q to quit)");
		else
			sprintf(buffer, "%2d MONSTERS LEFT !! (press A to use AI)", 
					nummon);

		ui_clearRow(DUNG_H+2);
		mvprintw(DUNG_H+2, 1, "%s", buffer);

		dungeon_print();

		refresh();

		if (pc->dead || !nummon) {
			char ch;
			while ((ch=getch())!='Q' && (ch!='q'));
			break;
		}

		int quit = 0;

		int ch = getch();

		// 1 if user does some invalid action 
		int invalid = 0;

		switch (ch) {
			case 'Q':
			case 'q':
				quit = 1;
				break;
			case 'A':
			case 'a':
				move_pc(); // automatic
				break;
				/* 1.05 */
			case '7':
			case 'y':
				invalid = move_c(pc, pc->x-1, pc->y-1);
				break;
			case '8':
			case 'k':
				invalid = move_c(pc, pc->x, pc->y-1);
				break;
			case '9':
			case 'u':
				invalid = move_c(pc, pc->x+1, pc->y-1);
				break;
			case '6':
			case 'l':
				invalid = move_c(pc, pc->x+1, pc->y);
				break;
			case '3':
			case 'n':
				invalid = move_c(pc, pc->x+1, pc->y+1);
				break;
			case '2':
			case 'j':
				invalid = move_c(pc, pc->x, pc->y+1);
				break;
			case '1':
			case 'b':
				invalid = move_c(pc, pc->x-1, pc->y+1);
				break;
			case '4':
			case 'h':
				invalid = move_c(pc, pc->x-1, pc->y);
				break;
			case '>':
				if (tmap[pc->y][pc->x]=='>')
					reset();
				else
					invalid = 1;
				break;
			case '<':
				if (tmap[pc->y][pc->x]=='<')
					reset();
				else
					invalid = 1;
				break;
			case ' ':
				break;
			case 'm':
				ui_mList();
				break;
			case 'S':
				// case 's':
				quit = 1;
				break;
			default:
				invalid = 1;
				break;
		}

		if (ch == 'm')
			continue;
		if (ch == STAIR_UP || ch == STAIR_DN)
			continue;
		if (invalid)
			continue;

		if (quit)
			break;

		while (1) {
			Character *c = turn_extract();

			if (c->dead) {
				C_delete(c);
				continue;
			}

			turn_insert(c);

			if (ISPC(c)) {
				break;
			} else {
				move_npc(c);
				if (pc->dead)
					break;
			}
		} // end of monsters turn
	} // end of game simulation

	endwin();

	turn_delete();

	if (savep)
		dungeon_save();

	for (i = 0; i < nummon; i++)
		free(npcs[i]);

	free(pc);

	return 0;
}

int reset() {
	int i;

	for (i = 0; i < nummon; i++)
		free(npcs[i]);	

	dungeon_clear();
	dungeon_generate();

	C_place_char(pc);

	// reset turn	
	turn_delete();
	turn_init();
	pc->turn = 0;

	turn_insert(pc);

	// reset monsters
	nummon = 10; 
	for (i=0; i<nummon; i++) {
		npcs[i] = C_create(0);
		C_place_char(npcs[i]);
		turn_insert(npcs[i]);
	}

	// init distance map
	dijkstra_init(pc->x, pc->y, 0);
	dijkstra_init(pc->x, pc->y, 1);

	return 0;
}

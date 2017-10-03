#include "core.h"
#include "corridor.h"
#include "character.h"
#include "debug.h"
#include "dungeon.h"
#include "move.h"
#include "room.h"
#include "turn.h"
#include "dijkstra.h"

const char *victory =
  "\n                                       o\n"
  "                                      $\"\"$o\n"
  "                                     $\"  $$\n"
  "                                      $$$$\n"
  "                                      o \"$o\n"
  "                                     o\"  \"$\n"
  "                oo\"$$$\"  oo$\"$ooo   o$    \"$    ooo\"$oo  $$$\"o\n"
  "   o o o o    oo\"  o\"      \"o    $$o$\"     o o$\"\"  o$      \"$  "
  "\"oo   o o o o\n"
  "   \"$o   \"\"$$$\"   $$         $      \"   o   \"\"    o\"         $"
  "   \"o$$\"    o$$\n"
  "     \"\"o       o  $          $\"       $$$$$       o          $  ooo"
  "     o\"\"\n"
  "        \"o   $$$$o $o       o$        $$$$$\"       $o        \" $$$$"
  "   o\"\n"
  "         \"\"o $$$$o  oo o  o$\"         $$$$$\"        \"o o o o\"  "
  "\"$$$  $\n"
  "           \"\" \"$\"     \"\"\"\"\"            \"\"$\"            \""
  "\"\"      \"\"\" \"\n"
  "            \"oooooooooooooooooooooooooooooooooooooooooooooooooooooo$\n"
  "             \"$$$$\"$$$$\" $$$$$$$\"$$$$$$ \" \"$$$$$\"$$$$$$\"  $$$\""
  "\"$$$$\n"
  "              $$$oo$$$$   $$$$$$o$$$$$$o\" $$$$$$$$$$$$$$ o$$$$o$$$\"\n"
  "              $\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\""
  "\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"$\n"
  "              $\"                                                 \"$\n"
  "              $\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\""
  "$\"$\"$\"$\"$\"$\"$\"$\n"
  "                                   You win!\n\n";

const char *tombstone =
  "\n\n\n\n                /\"\"\"\"\"/\"\"\"\"\"\"\".\n"
  "               /     /         \\             __\n"
  "              /     /           \\            ||\n"
  "             /____ /   Rest in   \\           ||\n"
  "            |     |    Pieces     |          ||\n"
  "            |     |               |          ||\n"
  "            |     |   A. Luser    |          ||\n"
  "            |     |               |          ||\n"
  "            |     |     * *   * * |         _||_\n"
  "            |     |     *\\/* *\\/* |        | TT |\n"
  "            |     |     *_\\_  /   ...\"\"\"\"\"\"| |"
  "| |.\"\"....\"\"\"\"\"\"\"\".\"\"\n"
  "            |     |         \\/..\"\"\"\"\"...\"\"\""
  "\\ || /.\"\"\".......\"\"\"\"...\n"
  "            |     |....\"\"\"\"\"\"\"........\"\"\"\"\""
  "\"^^^^\".......\"\"\"\"\"\"\"\"..\"\n"
  "            |......\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"......"
  "..\"\"\"\"\"....\"\"\"\"\"..\"\"...\"\"\".\n\n"
  "            You're dead.  Better luck in the next life.\n\n\n";

int pcx = 0; // 0 means unset
int pcy = 0; // 0 means unset

int cheat = 0;

Character *pc;
Character *npcs[1<<10];

char unify = 0;

int ai = 0;

unsigned int seed = 0;

int sight = 0;

int nummon = 10;
int nummonmax = 255;

int numstair = 2;

void io_init_terminal(void)
    {
      initscr();
      raw();
      noecho();
      curs_set(0);
      keypad(stdscr, TRUE);
      start_color();
    }

/* double checks if someone wants to quit, will only take Y or y for true, anything else will cancel */
int are_you_sure(void) {
	mvprintw(22, 0, "Are you sure you want to quit? (Y/N)");
	refresh();
	int ch = getch();
	if (ch == 'Y' || ch == 'y') {
		return 1;
	} else {
		return 0;
	}
}

int stairs_place() {
	int n, x, y;
	for(n = 0; n < numstair; n++){
		while (1) {
			x = rand() % DUNG_W;
			y = rand() % DUNG_H;

			if (tmap[y][x] == ROOM) {
				tmap[y][x] = (rand() % 2) ? STAIR_UP : STAIR_DN;
				break;
			}
		}
	}

	return 0;
}

int regenerate(void)
{
	int n, i, j;
	dungeon_generate();
	stairs_place();
	turn_delete();

	// Clear old cmap;
	for(i = 0; i < DUNG_H; i++) {
		for(j = 0; j < DUNG_W; j++) {
			cmap[i][j] = NULL;
		}
	}

	character_place(pc);
	// init turn heap
	turn_init();
	// insert PC into turn heap
	turn_insert(pc);

	

	// create monsters and insert them into turn heap
	for (n= 0; n < nummon; n++) {
		npcs[i] = character_new(0);
		character_place(npcs[i]);
		turn_insert(npcs[i]);
	}

	dungeon_print();
	return 0;
}

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
			}else if (!strcmp("--ai", argv[i]))
			{
				ai = 1;
			}
			else if (!strcmp("--numstair", argv[i]))
			{
				if (i+1<argc)
				{
					numstair = atoi(argv[++i]);
				}
				else
				{
					fprintf(stderr, "usage: rlg327 --numstair <int>\n");
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

	stairs_place();
	pc = character_new(1);	

	// place PC	
	if (!pcx && !pcy) {
		character_place(pc); // randomly place if unset
	} else {
		pc->x = pcx;
		pc->y = pcy;

		cmap[pcy][pcx] = pc;
	}

	// init turn heap
	turn_init();
	// insert PC into turn heap
	turn_insert(pc);

	// create monsters and insert them into turn heap
	for (i=0; i<nummon; i++) {
		npcs[i] = character_new(0);
		character_place(npcs[i]);
		turn_insert(npcs[i]);
	}

	// unify all monsters to one type
	if (unify) {
		// unify characteristics
		char unifyc = unify >= 'a' ? unify - 'a' + 10 : unify - '0'; 
		for (i=0; i<nummon; i++)
			npcs[i]->c = unifyc;
	}

	// Initialize ncurses
	io_init_terminal();
	int ch;
	// game simulation
	while (1) {
		dungeon_print();

		if (pc->dead){
			clear();
			printw(tombstone);
			break; // end of game
		} else if(!nummon) {
			clear();
			printw(victory);
			break; // end of game
		}
		
		// Input
		ch = getch();

		if (ch == 'Q') { // Quit game, don't let monsters move
			if (are_you_sure()){
					endwin();
					exit(0);
			}
			continue;
		} else if (ch == 'm') { // Open monster's list, don't let monsters move
			print_characters();
			continue;
		}
		if (ai) {
			move_pc();
		} else {
			if (ch == '7' || ch == 'y') { // Move top left
				if (move_c(pc, pc->x-1, pc->y-1)) continue;
			} else if (ch == '8' || ch == 'k') { // Move up
				if (move_c(pc, pc->x  , pc->y-1)) continue;
			} else if (ch == '9' || ch == 'u') { // Move top right
				if (move_c(pc, pc->x+1, pc->y-1)) continue;
			} else if (ch == '6' || ch == 'l') { // move right
				if (move_c(pc, pc->x+1, pc->y  )) continue;
			} else if (ch == '3' || ch == 'n') { // move bottom right
				if (move_c(pc, pc->x+1, pc->y+1)) continue;
			} else if (ch == '2' || ch == 'j') { // Move down
				if (move_c(pc, pc->x  , pc->y+1)) continue;
			} else if (ch == '1' || ch == 'b') { // Move bottom left
				if (move_c(pc, pc->x-1, pc->y+1)) continue;
			} else if (ch == '4' || ch == 'h') { // Move left
				if (move_c(pc, pc->x-1, pc->y  )) continue;
			} else if (ch == '>' && STAIR_DN == tmap[pc->y][pc->x]) { // Had code that checked if ch == tmap[pc->y][pc->x]
				regenerate();
			
			} else if (ch == '<' && STAIR_UP == tmap[pc->y][pc->x]) {
				regenerate();
			} else if (ch == '5' || ch == ' ') { // Take break
				// Do nothing
			} else { // Unrecognized key
				continue;
			}
		} 

		// move monsters
		while (1) {
			Character *c = turn_extract();

			if (c->dead) {
				character_delete(c);
				continue;
			}

			turn_insert(c);

			if (ISPC(c)) {
				break; // break on PC turn
			} else {
				move_npc(c);
				if (pc->dead){
					break;
				}
				
			}
		} // end of monsters turn
	} // end of game simulation
	refresh();
	turn_delete();

	if (savep)
		dungeon_save();

	for (i = 0; i < nummon; i++)
		free(npcs[i]);

	free(pc);
	getch();
	endwin();

	return 0;
}


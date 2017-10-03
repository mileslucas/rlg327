#include "character.h"
#include "dungeon.h"

Character::Character(int isPC) {
	if (isPC){
		ch = PC_C;
		speed = 10;
	} else {
		ch = rand() & 0xf;
		speed = 5 + (rand() & 0xf); // 5-20 for monsters
	}
	dead = 0;
	turn = 0;
	mempcx = 0;
	mempcy = 0;
}

char Character::get_char() {
	if (ISPC(this)) 
		return '@';
	else if (ch < 10)
		return '0' + ch;
	else if (ch < 16)
		return 'a' + ch - 10;
	else
		return -1;
}


int Character::get_color() {
    int color = 0;
    if (ISPC(this)) 
    	color = COLOR_PC;
    else if (speed <= 8)
		color = COLOR_WHITE;
	else if (speed <= 12)
		color = COLOR_GREEN;
	else if (speed <= 16)
		color = COLOR_CYAN;
	else if (speed <= 20)
		color = COLOR_MAGENTA;
	
	return color;
}


void Character::place_char() {
	while (1) {
		int x = rand() % DUNG_W;
		int y = rand() % DUNG_H;

		if (!hmap[y][x]) {
			cmap[y][x] = this;

			this->x = x;
			this->y = y;
			break;
		}
	}

	if (ISPC(this))
		pc = this;

}

Character::~Character(){
	int i;
	if (!ISPC(this)) {
		// make sure deleted character not in [0, nummon) of npcs
		for (i=0; i<nummon; i++) {
			if (npcs[i]==this) {
				npcs[i] = npcs[--nummon];
				break;
			}
		}
	}
	delete this;
}

extern "C" void* C_create(int isPC) {
	return (void*) new Character(isPC);
}
extern "C" void C_delete(Character *c) {
	delete c;
}
extern "C" char C_get_char(Character *c) {
	return c->get_char();
}
extern "C" int C_get_color(Character *c) {
	return c->get_color();
}
extern "C" void C_place_char(Character *c){
	c->place_char();
}


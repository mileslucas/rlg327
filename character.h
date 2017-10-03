#ifndef CHARACTER_H
#define CHARACTER_H

#include "core.h"

#define PC_C 0x10 // PC characteristic
#define ISPC(cp) (cp->ch & PC_C)

class Character {
public:
  char ch;
  int speed;
  int x;
  int y;
  int mempcx;
  int mempcy;
  // 1 if dead
  int dead;
  int turn;

  Character(int isPC);
  ~Character();
  char get_char();
  int get_color();
  void place_char();

};

#ifdef __cplusplus
extern "C" {
#endif  

extern Character *pc;
extern Character *npcs[];
extern int nummon;

Character *cmap[DUNG_H][DUNG_W];

void* C_create(int isPC);
void C_delete(Character *c);
char C_get_char(Character *c);
int C_get_color(Character *c);
void C_place_char(Character *c);


#ifdef __cplusplus
}
#endif

#endif


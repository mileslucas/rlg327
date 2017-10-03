#ifndef TURN_H
#define TURN_H

#include "character.h"

int turn_init();

int turn_delete();

int turn_insert(Character*);

Character *turn_extract();

#endif

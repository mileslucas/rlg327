#include "core.h"
#include "debug.h"
#include "heap.h"
#include "character.h"

Heap h;

static int compare(const void *cv, const void *dv)
{
	Character *c = (Character *) cv;
	Character *d = (Character *) dv;

	if (c->turn == d-> turn)
		return (!ISPC(c)) - (!ISPC(d)); // PC has higher priority
	else
		return c->turn - d->turn;
}

int turn_init()
{
	heap_init(&h, compare);
	return 0;
}

int turn_delete()
{
	heap_delete(&h);
	return 0;
}

int turn_insert(Character *c)
{
	c->turn += (100/c->speed);
	heap_insert(&h, c);
	return 0;
}

Character *turn_extract()
{
	return heap_extract(&h);
}


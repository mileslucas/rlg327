#ifndef PC_H
#define PC_H

#include "character.h"

class PC : public Character
{
	public:
		PC();
		~PC();

		NPC *attacking; // currently attacking
		
		char *seenDungeon;
			
		void clearSeenDungeon();
		
		inline char *getSeenDungeon() { return seenDungeon; }

		void setLocation(int x, int y);
};

extern PC *pc;

#endif


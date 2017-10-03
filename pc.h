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
		
		int getSpeed();

		int getTotalDam();

		int getTotalDamRanged();

		int getTotalDef();

		int hpmax(); // naked hpmax plus item hit bonus
};

extern PC *pc;

#endif


#ifndef CHARACTER_H
#define CHARACTER_H

#include "core.h"
#include "dice.h"
#include "point.h"

class Character
{
	protected:
		bool _isPC;

		int _hpmax;
		int _mpmax;

		int x, y;
		
		bool dead;
		
		char symb;
		
		int color;
		int speed;
		int turn;

	public:
		Character();
		virtual ~Character();
		
		// getters and setters for (x, y)
		Point getLocation();
		void getLocation(int *, int *);
		virtual void setLocation(int x, int y);
		inline int getX() { return x; }
		inline int getY() { return y; }
		inline void setX(int x) { setLocation(x, y); }
		inline void setY(int y) { setLocation(x, y); }
	
		int getColor();	
		
		virtual int hpmax() { return _hpmax; } 
		virtual int mpmax() { return _mpmax; }

		inline bool isDead() { return dead; }
		inline void setDead() { dead = true; }
		
		// true if this character is PC
		inline bool isPC() { return _isPC; }
	
		// speed
		virtual int getSpeed() { return speed; }
		
		// symbol
		inline char getSymb() { return symb; } 
	
		// turn	
		inline int  getTurn() { return turn; }
		inline void setTurn(int turn) { this->turn = turn; }
};

#endif


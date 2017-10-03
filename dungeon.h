#ifndef DUNG_HH
#define DUNG_HH

#include <vector>

#include "character.h"
#include "npc.h"
#include "room.h"
#include "turn.h"
#include "object.h"
#include "objectfactory.h"


using namespace std;

class Dungeon
{
private:
	vector<NPC *> deadnpcv;

	int getNPCIndex(NPC *);

public:
	// terrain
	char tmap[DUNG_H][DUNG_W];
	// hardness
	unsigned char hmap[DUNG_H][DUNG_W];
	// characters
	Character *cmap[DUNG_H][DUNG_W];
	// objects
	Object *imap[DUNG_H][DUNG_W];

	vector<NPC *> npcv;

	vector<NPC *> vnpcv; // visible monsters

	vector<Object *> obv;

	vector<Room *> roomv;

	Turn *turn;

	Dungeon();
	~Dungeon();

	int fill(char);
	int generate();

	// true if (x, y) is visible by PC
	bool isVisible(int x, int y);

	// true if line of sight is not blocked between two locations
	bool isVisible(int x, int y, int ox, int oy);

	inline int nummon() { return  npcv.size(); }
	inline int numobj() { return obv.size(); }

	int removeMonster(NPC *);

	int printDungeon();

	int placeCharacter(Character *);
	int placeCharacter(Character *, int x, int y);
	int placeObject(Object *);

	int load(const char *path);
	int save(const char *path);

	// old version - generate 0 ~ f
	int generateRandMonsters(int nummon);

	// new version - generate from factories
	int generateMonsters(int nummon);
	int generateObjects(int numobj);
};

// current active dungeon
extern Dungeon *dungeon;

#endif

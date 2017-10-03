#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sstream>

#include "core.h"
#include "dice.h"
#include "debug.h"
#include "monsterfactory.h"
#include "parser.h"
#include "ui.h"

int Parser::parse(const char *path)
{
	ifstream ifs(path);

	if (!ifs) {
		cerr << "Failed to open " << path << endl;
		return -1;
	}
	
	if (ifs.eof())
		return -1;

	// meta
	string line;
	getline(ifs, line);
	if (line=="RLG327 MONSTER DESCRIPTION 1") {
		getline(ifs, line);
		while (!ifs.eof()) {
			MonsterFactory *mf = MonsterFactory::next(ifs);
			if (mf != NULL) 
				cout << *mf << endl;
		}
		ifs.close();
		return 0;
	}
	
	ifs.close();
	return -1;
}

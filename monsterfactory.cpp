#include <stdlib.h>

#include <iostream>
#include <sstream>

#include "debug.h"
#include "monsterfactory.h"
#include "parser.h"

using namespace std;

MonsterFactory *MonsterFactory::next(istream &is)
{
	string line;
	getline(is, line);
	while(line != "BEGIN MONSTER"){
		getline(is, line);
	}
	MonsterFactory *mf = new MonsterFactory;
	init_mf(mf);

	getline(is, line);
	while (line != "END") {
		size_t del = line.find(' ');
		string token = line.substr(0, del);
		if (token == "NAME") {
			if (mf->name == ""){
				mf->name = line.substr(del+1);
			} else {
				cerr << "Invalid monster description" << endl;
				return NULL;
			}
			
		} else if (token == "SYMB") {
			if (mf->symb == 0){
				mf->symb = line[del+1];
			} else {
				cerr << "Invalid monster description" << endl;
				return NULL;
			}
		} else if (token == "COLOR") {
			if (mf->color == 0){
				mf->color = get_color(line.substr(del+1));
			} else {
				cerr << "Invalid monster description" << endl;
				return NULL;
			}
		} else if (token == "DESC") {
			if (mf->desc == ""){
				getline(is, line);
				while(line != ".") {
					mf->desc += line;
					getline(is, line);
				}
			} else {
				cerr << "Invalid monster description" << endl;
				return NULL;
			}
			
		} else if (token == "ABIL") {
			if (mf->abil == ""){
				mf->abil = line.substr(del+1);
			} else {
				cerr << "Invalid monster description" << endl;
				return NULL;
			}
			
		} else if (token == "SPEED") {
			if (mf->dspeed == NULL){
				string d = line.substr(del+1);
				mf->dspeed = Dice::parseDice(d);
			} else {
				cerr << "Invalid monster description" << endl;
				return NULL;
			}
		} else if (token == "HP") {
			if (mf->dhp == NULL){
				string d = line.substr(del+1);
				mf->dhp = Dice::parseDice(d);
			} else {
				cerr << "Invalid monster description" << endl;
				return NULL;
			}
		} else if (token == "DAM") {
			if (mf->ddam == NULL){
				string d = line.substr(del+1);
				mf->ddam = Dice::parseDice(d);
			} else {
				cerr << "Invalid monster description" << endl;
				return NULL;
			}
		}
		getline(is, line);
	}
	
	// Check that all fields are filled
	if (mf->name == "" || mf->desc == "" || mf->symb == 0 || mf->color == 0 || mf->dspeed == NULL || mf->abil == "" || mf->dhp == NULL || mf->ddam == NULL){
		cerr << "Invalid monster description" << endl;
		return NULL;
	}
	return mf;
	
		
}

void init_mf(MonsterFactory *mf) {
	mf->name = "";
	mf->desc = "";
	mf->symb = 0;
	mf->color = 0;
	mf->abil = "";

	mf->dspeed = NULL;
	mf->dhp = NULL;
	mf->ddam = NULL;
}

int get_color(string s) {
	if (s == "BLACK")
		return 0;
	else if (s == "RED")
		return 1;
	else if (s == "GREEN")
		return 2;
	else if (s == "YELLOW")
		return 3;
	else if (s == "BLUE")
		return 4;
	else if (s == "MAGENTA")
		return 5;
	else if (s == "CYAN")
		return 6;
	else if (s == "WHITE")
		return 7;
	else
		return -1;
}

string get_color(int c) {
	if (c == 0)
		return "BLACK";
	else if (c == 1)
		return "COLOR_RED";
	else if (c == 2)
		return "COLOR_GREEN";
	else if (c == 3)
		return "COLOR_YELLOW";
	else if (c == 4)
		return "COLOR_BLUE";
	else if (c == 5)
		return "COLOR_MAGENTA";
	else if (c == 6)
		return "COLOR_CYAN";
	else if (c == 7)
		return "COLOR_WHITE";
	else
		return "";
}

ostream& operator<<(ostream& os, MonsterFactory &mf)
{
	os << mf.name  << endl;
	os << mf.desc  << endl;
	os << mf.symb  << endl;
	os << get_color(mf.color) << endl;
	os << *mf.dspeed << endl;
	os << mf.abil  << endl;
	os << *mf.dhp    << endl;
	os << *mf.ddam   << endl;
	return os;
}

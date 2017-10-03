#ifndef UI_H
#define UI_H

class UI
{
	public:
		static int initColors();

		// called when --help is specified
		static int help();

		static int clearRow(int row);
		static int reprint();
		
		static int mList();
};

#endif

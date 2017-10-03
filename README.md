Assignment 1.03 Dungeon Creation
Miles Lucas <mdlucas@iastate.edu>
Com S 327 Iowa State University
Department of Computer Science

This program forms a basis for a Roguelike game played on the terminal. It creates a dungeon with rooms and hallways. Underneath of that is a hardness scale which defines the hardness of the rock walls. In addition, in order to facilitate intelligent monster movement, there are two maps with the distance to the character, one for tunneling creatures and one for non-tunneling creatures. This program has multiple command line parameters.

--rand | -r <Optional: integer seed>
	This will seed the random number generator with the given integer seed

--save | -s
	This will save the current dungeon into a .rlg327 file in the Home directory. If there is already a file there it will be overwritten

--load | -l <Optional: full filepath>
	This will load a dungeon at the given filepath into the game. If no filepath is provided it will load the file at the /home/.rlg327/dungeon directory

--image | -i <full filepath>
	This will load a dungeon based on a given .pgm file where the darker the tone, the less the hardness is, with pure black being 0 hardness.  
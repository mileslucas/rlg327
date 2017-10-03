Miles Lucas
mdlucas@iastate.edu
Com S 327 Department of Computer Science
Iowa State University

RLG 327

A mimic of the classic rouge-like game for Com S 327. This program uses Ncurses to allow the user to control a player and travel around a dangerous dungeon filled with spooky monsters. The monsters have various abilities coordinated in hexadecimal with their character. Here is the chart for all the characters:

*Intelligent (MSB)
	-If a monster is intelligent it will walk towards PC if visible and remembers last known location. Also uses most efficient path
*Telepathic
	-If a monster is telepathic it always knows where the PC is
*Tunneling
	-If a monster can tunnel it can break through the rock walls to create hallways
*Erratic (LSB)
	-If a monster is erratic, it has a 50% chance to move randomly instead of its prescribed movement

So, the hexadecimal representation corresponds to a byte of binary data with 1 signifying the characteristic.For example, the character 9 in binary is 0101 so that monster is telepathic and erratic.

The game will allow the user to travel throughout the map using the following key bindings:
Key(s)    |  Action
7 or y      Attempt to move PC one cell to the upper left.
8 or k      Attempt to move PC one cell up.
9 or u      Attempt to move PC one cell to the upper right.
6 or l      Attempt to move PC one cell to the right.
3 or n      Attempt to move PC one cell to the lower right.
2 or j      Attempt to move PC one cell down.
1 or b      Attempt to move PC one cell to the lower left.
4 or h      Attempt to move PC one cell to the left.
>           Attempt to go down stairs.
<           Attempt to go up stairs.
5 or space  Rest for a turn. NPCs still move.
m           Display a list of monsters in the dungeon, with their symbol and position relative to
             the PC (e.g.: “c, 2 north and 14 west”).
up arrow    When displaying monster list, if entire list does not fit in screen and not currently at
             top of list, scroll list up.
down arrow  When displaying monster list, if entire list does not fit in screen and not currently at
             bottom of list, scroll list down.
escape      When displaying monster list, return to character control
Q           Quit the game

The game is over when either the PC is attacked by a monster, or when all the monsters are killed. 

The game takes the following command line arguments:
--load (Optional:)<filepath>    Loads a dungeon at the filepath. Takes files of type .rlg327
--save (Optional:)<filepath>    Saves a dungeon at the filepath.
--nummon <num>                  Specifies the number of monsters. Must be less than 255
--cheat                         Makes the PC invincible
--pc <x> <y>                    Specifies the starting location of the pc
--seed <seed>                   Specifies the seed for the random function
--sight                         Visualizes the lines of sight for the monsters
--unify <hex char>              Unifies the monsters as all one type
--ai                            Let's Yiuxiang's ai take over control of the PC
--parse (Optional:)<filepath>   Parses the monster description from the filepath

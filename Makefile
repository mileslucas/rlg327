dungeon: dungeon.c
	@gcc -Wall -Werror -ggdb dungeon.c -o dungeon
	@echo compiled dungeon.c

clean:
	@rm -f *~ *.o dungeon
	@echo cleaned up files


CFLAGS = -g -Wall -Werror 

OBJS = \
character.o \
corridor.o \
debug.o \
dijkstra.o \
dungeon.o \
heap.o \
main.o \
move.o \
point.o \
room.o \
turn.o \
ui.o \


NAME = rlg327

all: $(NAME)

clean:
	rm -rf $(NAME) *.o $(NAME).dSYM .DS_Store

$(NAME): $(OBJS) 
	g++ $(CFLAGS) -o $(NAME) $(COBJS) -lncurses

%.o: %.c
	g++ $(CFLAGS) -c $<

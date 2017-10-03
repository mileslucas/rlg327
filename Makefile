CFLAGS := -g -Wall -Werror 

OBJS := \
character.o \
core.o \
corridor.o \
debug.o \
dice.o \
dijkstra.o \
dungeon.o \
heap.o \
main.o \
monsterfactory.o \
move.o \
npc.o \
object.o \
objectfactory.o \
parser.o \
pc.o \
point.o \
room.o \
turn.o \
ui.o \

NAME := rlg327

all: $(NAME)

clean:
	@$(RM) -f $(NAME) *.o .DS_Store

$(NAME): $(OBJS) 
	@g++ $(CFLAGS) -o $(NAME) $(OBJS) -lncurses

%.o: %.cpp
	@g++ $(CFLAGS) -c $<

BASE := ../lucas_miles.assignment-
NUM := 1.08
TARDIR := $(BASE)$(NUM)
DIR := $(TARDIR)/
tar: 
	@mkdir $(DIR)
	@git log --graph | tee $(DIR)/CHANGELOG
	@rsync -av --exclude='.*' --exclude='save_files' '../assignment1.08/' $(DIR)
	@tar cvfz $(TARDIR).tar.gz $(DIR)
	@rm -rf $(DIR)



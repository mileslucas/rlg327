CFLAGS := -g -Wall -Werror 
CC := g++


INC := -I include
SRCDIR := src
BUILDDIR := build
LIB := -lncurses

NAME := rlg327

SOURCES := $(shell find $(SRCDIR) -type f -name *.cpp)
OBJS := $(patsubst $(SRCDIR), $(BUILDDIR), $(SOURCES:.CPP=.O))

all: $(NAME)

clean:
	rm -rf $(NAME) $(BUILDDIR) .DS_Store

$(NAME): $(OBJS) 
	$(CC) $(CFLAGS) $(INC) -o $@  $^ $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(BUILDDIR)
	g++ $(CFLAGS) $(INC) -o $@ -c $<

BASE := ../lucas_miles.assignment-
NUM := 1.09
TARDIR := $(BASE)$(NUM)
DIR := $(TARDIR)/
tar: 
	@mkdir $(DIR)
	@git log --graph | tee $(DIR)/CHANGELOG
	@rsync -av --exclude='.*' --exclude='save_files' '../assignment1.08/' $(DIR)
	@tar cvfz $(TARDIR).tar.gz $(DIR)
	@rm -rf $(DIR)

.PHONY: clean

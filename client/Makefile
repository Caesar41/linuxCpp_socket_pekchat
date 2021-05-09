#!Makefile

PROGRAM = pekchat_client

C_SOURCES = $(shell find . -name "*.cpp")
C_OBJECTS = $(patsubst %.cpp, %.o, $(C_SOURCES))

CC = g++

C_FLAGS = -DDEBUG -c -Wall -Wextra -ggdb3 -I.

all: $(C_OBJECTS)
	@echo link...
	$(CC) $(C_OBJECTS) -lm -lpthread -lmysqlclient -o $(PROGRAM)

.c.o:
	@echo compile $< ...
	$(CC) $(C_FLAGS) $< -o $@

.PHONY:clean
clean:
	$(RM) $(C_OBJECTS) pekchat_client
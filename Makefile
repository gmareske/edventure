CC=gcc
CFLAGS=-g -Wall -Wextra -pedantic -std=c11
EXEC=edv

SOURCES=main.c game.c list.c
OBJECTS=$(SOURCES:.c=.o)

# target_name:  dependency_list
#	command(s)
all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXEC)

# $(EXEC): 
# 	$(CC) $(CFLAGS) $(SOURCES) -o $(EXEC)

# List program components, what they depend on, and how to compile or link each

#list:  list.o list-proc.o
#	$(CC) -o $@ $^

.PHONY: clean

clean:
	rm -f $(EXEC) *.out *.o *~ core*


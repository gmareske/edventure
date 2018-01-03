CC = gcc
CFLAGS = -g -Wall -Wextra -pedantic -std=c11

# target_name:  dependency_list
#	command(s)


# List program components, what they depend on, and how to compile or link each

#list:  list.o list-proc.o
#	$(CC) -o $@ $^

.PHONY: clean

clean:
	rm -f list *.o *~ core*

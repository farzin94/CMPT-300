# Define what compiler to use and the flags.
CC=gcc
CXX=g++
CCFLAGS= -g -Wall -Werror -std=c99 -pthread

SRC = candykids.c 


# Compile all .c files into .o files
# % matches all (like * in a command)
# $< is the source file (.c file)
%.o : %.c
	$(CC) -c $(CCFLAGS) $<

candykids: $(SRC)
	gcc -o candy $(CCFLAGS) $(SRC)

clean:
	rm -f candy *.o
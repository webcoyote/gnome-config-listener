NAME=gnome-config-listener
CFLAGS=-g -Wall -o $(NAME)
GTK_FLAGS=-export-dynamic `pkg-config --cflags --libs gtk+-2.0 gconf-2.0`
CC=gcc

SRCS=gnome-config-listener.c

# top-level rule to create the program.
all: main

# compiling the source file.
main: $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) $(GTK_FLAGS)

# cleaning everything that can be automatically recreated with "make".
clean:
	/bin/rm -f $(NAME) *.o

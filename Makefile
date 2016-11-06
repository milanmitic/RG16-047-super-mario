PROGRAM = pacman
CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -lGL -lGLU -lglut

$(PROGRAM): main.o
		$(CC) -o $(PROGRAM) main.o $(LDFLAGS)

.PHONY: clean dist

clean:
	-rm *.o $(PROGRAM) *core

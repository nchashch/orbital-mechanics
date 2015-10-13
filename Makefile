CC=clang
LIBS=`sdl2-config --libs` -lm -lGL
CFLAGS=`sdl2-config --cflags` -I/usr/include/GL/
SRC=main.c frame.c vector.c matrix.c transformation.c object.c
all:
	$(CC) $(SRC) $(LIBS) $(CFLAGS)
clean:
	rm a.out

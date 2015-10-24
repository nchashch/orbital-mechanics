CC=clang++
LIBS=`sdl2-config --libs` -lm -lGL
CFLAGS=`sdl2-config --cflags` -I/usr/include/GL/ -I./src/glm/
SRC=src/*.cpp src/tol/*.cc
all:
	$(CC) $(SRC) $(LIBS) $(CFLAGS)
clean:
	rm a.out

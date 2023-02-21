
all: lab2 
lab2: xylab2.cpp xreyes.cpp abotello.cpp abotello.h
	g++ xylab2.cpp xreyes.cpp abotello.cpp abotello.h libggfonts.a -Wall -olab2 -lX11 -lGL -lGLU -lm
clean:
	rm -f lab2


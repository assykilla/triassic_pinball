
all: lab2 
lab2: xylab2.cpp xreyes.cpp abotello.cpp abotello.h isamara.cpp isamara.h
	g++ xylab2.cpp xreyes.cpp abotello.cpp isamara.cpp libggfonts.a -Wall -olab2 -lX11 -lGL -lGLU -lm
clean:
	rm -f lab2


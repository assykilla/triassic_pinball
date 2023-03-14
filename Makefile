
all: triassic_pinball 
triassic_pinball: triassic_pinball.cpp xreyes.cpp abotello.cpp abotello.h isamara.cpp isamara.h gtorres.cpp
	g++ triassic_pinball.cpp xreyes.cpp abotello.cpp isamara.cpp gtorres.cpp libggfonts.a -Wall -otriassic_pinball -lX11 -lGL -lGLU -lm
clean:
	rm -f triassic_pinball


main: main.o jb.o
	g++ -o main main.o jb.o -lncurses
main.o: main.cpp jb.h
	g++ -c main.cpp
jb.o: jb.cpp jb.h
	g++ -c jb.cpp
	


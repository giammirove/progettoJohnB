main: main2.o JohnB.o
	g++ -o main main2.o JohnB.o -lncurses
main2.o: main2.cpp JohnB.h
	g++ -c main2.cpp
JohnB.o: JohnB.cpp JohnB.h
	g++ -c JohnB.cpp
	


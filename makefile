test : main.o map.o player.o listaOggetto.o oggetto.o
	g++ -o test main.o map.o player.o listaOggetto.o oggetto.o -lncursesw

main.o : main.cpp map.h player.h oggetto.h
	g++ -c main.cpp -lncursesw

map.o : map.cpp map.h oggetto.h
	g++ -c map.cpp -lncursesw

player.o : player.cpp player.h oggetto.h
	g++ -c player.cpp -lncursesw

listaOggetto.o : listaOggetto.cpp listaOggetto.h oggetto.h
	g++ -c listaOggetto.cpp -lncursesw

oggetto.o : oggetto.cpp oggetto.h
	g++ -c oggetto.cpp -lncursesw

clean :
	rm *.o test
test : main.o map.o player.o listaOggetto.o oggetto.o convertiAsciiArt.o utility.o gestoreMovimento.o
	g++ -o test main.o map.o player.o listaOggetto.o oggetto.o convertiAsciiArt.o utility.o gestoreMovimento.o -lncursesw

main.o : main.cpp map.h player.h oggetto.h gestoreMovimento.h
	g++ -c main.cpp -lncursesw

convertiAsciiArt : convertiAsciiArt.cpp convertiAsciiArt.h utility.h
	g++ -c convertiAsciiArt.cpp -lncursesw

map.o : map.cpp map.h oggetto.h
	g++ -c map.cpp -lncursesw

player.o : player.cpp player.h oggetto.h
	g++ -c player.cpp -lncursesw

listaOggetto.o : listaOggetto.cpp listaOggetto.h oggetto.h
	g++ -c listaOggetto.cpp -lncursesw

oggetto.o : oggetto.cpp oggetto.h convertiAsciiArt.h utility.h
	g++ -c oggetto.cpp -lncursesw

utility.o : utility.cpp utility.h
	g++ -c utility.cpp -lncursesw

gestoreMovimento.o : gestoreMovimento.cpp gestoreMovimento.h player.h listaOggetto.h map.h utility.h
	g++ -c gestoreMovimento.cpp -lncursesw

clean :
	rm *.o test
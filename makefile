test : main.o map.o player.o listaOggetto.o oggetto.o convertiAsciiArt.o utility.o gestoreMovimento.o gestoreMondo.o listaNemici.o nemico.o weapon.o
	g++ -o test main.o map.o player.o listaOggetto.o oggetto.o convertiAsciiArt.o utility.o gestoreMovimento.o gestoreMondo.o listaNemici.o nemico.o weapon.o -lncursesw

main.o : main.cpp map.h player.h oggetto.h gestoreMovimento.h gestoreMondo.h listaNemici.h nemico.h
	g++ -c main.cpp -lncursesw

convertiAsciiArt : convertiAsciiArt.cpp convertiAsciiArt.h utility.h
	g++ -c convertiAsciiArt.cpp -lncursesw

map.o : map.cpp map.h oggetto.h
	g++ -c map.cpp -lncursesw

player.o : player.cpp player.h oggetto.h convertiAsciiArt.h weapon.h
	g++ -c player.cpp -lncursesw

listaOggetto.o : listaOggetto.cpp listaOggetto.h oggetto.h
	g++ -c listaOggetto.cpp -lncursesw

oggetto.o : oggetto.cpp oggetto.h convertiAsciiArt.h utility.h
	g++ -c oggetto.cpp -lncursesw

utility.o : utility.cpp utility.h
	g++ -c utility.cpp -lncursesw

gestoreMovimento.o : gestoreMovimento.cpp gestoreMovimento.h player.h listaOggetto.h map.h utility.h
	g++ -c gestoreMovimento.cpp -lncursesw

gestoreMondo.o : gestoreMondo.cpp gestoreMondo.h oggetto.h convertiAsciiArt.h utility.h
	g++ -c gestoreMondo.cpp -lncursesw

listaNemici.o : listaNemici.cpp listaNemici.h 
	g++ -c listaNemici.cpp -lncursesw

nemico.o: nemico.cpp nemico.h oggetto.h map.h 
	g++ -c nemico.cpp -lncursesw

weapon.o: weapon.cpp weapon.h oggetto.h map.h utility.h
	g++ -c weapon.cpp -lncursesw

clean :
	rm *.o test
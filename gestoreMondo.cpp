#include <string.h>
#include <locale.h>
#include <ncurses.h>
#include <cstdlib>
#include "gestoreMondo.h"
#include "oggetto.h"

GestoreMondo::GestoreMondo(ConvertiAsciiArt *asciiArt) {
    _width = 5; 
    _map_h = 18;
    _max_h = 10;
    _min_h = 2;
    _saltoPlayer = 5;
}

GestoreMondo::GestoreMondo (int _w, int map_h, int max_h, int min_h, int saltoPlayer, ConvertiAsciiArt *asciiArt) {
    _width = _w;
    _map_h = map_h;
    _max_h = max_h;
    _min_h = min_h;
    _saltoPlayer = saltoPlayer;
    _asciiArt = asciiArt;
}

Oggetto *GestoreMondo::generaOggetto () {
    int rnd = rand() % 2;
    int start_x = 0;
    if(_oggettoPrec != NULL)
        start_x = _oggettoPrec->ottieniFigura()->x;
    else 
        start_x = 4;
    int rnd_x = start_x + rand () % _width;
    // abbiamo dato per scontato il salto del player come costante
    int rnd_y = -1;
    while(rnd_y < _saltoPlayer || rnd_y > _map_h) {
        if(_oggettoPrec != NULL)
            rnd_y = ((_oggettoPrec->ottieniFigura()-> y - _saltoPlayer) + rand () % (2 * _saltoPlayer));
        else 
            rnd_y = _map_h - _saltoPlayer;
    }
    // ci saranno dei controlli qua per rnd_x e rnd_y
    Oggetto *tmp = new Oggetto(rnd_x, rnd_y, (TipoDiOggetto)rnd, _asciiArt);
    _oggettoPrec = tmp;

    mvprintw(0,0, "%d - %d", rnd_x, rnd_y);

    return tmp;
}
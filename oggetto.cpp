#include <string.h>
#include <locale.h>
#include <ncurses.h>
#include "oggetto.h"
#include "convertiAsciiArt.h"
#include "utility.h"

int Oggetto::_id = 0;

Oggetto::Oggetto(int x, int y, TipoDiOggetto tipo, ConvertiAsciiArt *asciiArt)
{
    _x = x;
    _y = y;
    _solido = false;

    impostaFigura(tipo, asciiArt);
    _myId = _id++;
}

int Oggetto::getId()
{
    return _myId;
}

int Oggetto::getClock() {
    return _clock;
}

bool Oggetto::getSolido() {
    return _solido;
}

TipoDiOggetto Oggetto::getTipoDiOggetto() {
    return _tipo;
}

void Oggetto::impostaFigura(TipoDiOggetto tipo, ConvertiAsciiArt *asciiArt)
{
    _figura = new figura_t;
    _tipo = tipo;

    switch (tipo)
    {
    case OS_ALBERO:
        aggiungiPuntoAFigura(&_figura, _x, _y, "\U0001F333");
        aggiungiPuntoAFigura(&_figura, _x+1, _y, " ");
        _clock = 100000;
        _solido = true;
        break;
    case OS_CESPUGLIO:
        aggiungiPuntoAFigura(&_figura, _x, _y, "o");
        //aggiungiPuntoAFigura(&_figura, _x+1, _y, " ");
        _clock = 100000;
        _solido = true;
        break;
    case OS_KIRBY:
        aggiungiPuntoAFigura(&_figura, _x, _y, "\U0001F416");
        aggiungiPuntoAFigura(&_figura, _x+1, _y, " ");
        _clock = 100000;
        _solido = true;
        break;
    case OS_PIATTAFORMA:
        aggiungiPuntoAFigura(&_figura, _x, _y, "=");
        aggiungiPuntoAFigura(&_figura, _x+1, _y, "=");
        aggiungiPuntoAFigura(&_figura, _x+2, _y, "=");
        _clock = 100000;
        _solido = true;
        break;
    case OS_WINDMILL:
        if(_figura != NULL) delete _figura;
        _figura = NULL;
        figura t = asciiArt->getFigura("WINDMILL");
        while (t != NULL) {
            aggiungiPuntoAFigura(&_figura, t->x + _x, t->y+_y, t->c);
            t = t->next;
        }
        _clock = 100000;
        _solido = true;
    break;
    }
}

figura Oggetto::ottieniFigura()
{
    return _figura;
}

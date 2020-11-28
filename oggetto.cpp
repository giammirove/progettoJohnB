#include <string.h>
#include <locale.h>
#include "oggetto.h"

int Oggetto::_id = 0;

Oggetto::Oggetto(int x, int y, TipoDiOggetto tipo)
{
    _x = x;
    _y = y;
    _solido = false;

    impostaFigura(tipo);
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

void aggiungiPuntoAFigura(figura *_figura, int x, int y, const char c[])
{
    if (_figura == NULL)
    {
        *_figura = new figura_t;
        (*_figura)->x = x;
        (*_figura)->y = y;
        strcpy((*_figura)->c, c);
        (*_figura)->next = NULL;
    }
    else
    {
        figura t = new figura_t;
        t->x = x;
        t->y = y;
        strcpy(t->c, c);
        t->next = (*_figura);
        (*_figura) = t;
    }
}

void Oggetto::impostaFigura(TipoDiOggetto tipo)
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
        aggiungiPuntoAFigura(&_figura, _x, _y, "\U0001F33F");
        aggiungiPuntoAFigura(&_figura, _x+1, _y, " ");
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
        aggiungiPuntoAFigura(&_figura, _x, _y, "\u25FC");
        aggiungiPuntoAFigura(&_figura, _x+1, _y, "\u25FC");
        aggiungiPuntoAFigura(&_figura, _x+2, _y, "\u25FC");
        _clock = 100000;
        _solido = true;
        break;
    }
}

figura Oggetto::ottieniFigura()
{
    return _figura;
}

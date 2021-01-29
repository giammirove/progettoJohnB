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

int Oggetto::getClock()
{
    return _clock;
}

bool Oggetto::getSolido()
{
    return _solido;
}

int Oggetto::getWidth()
{
    return _width;
}

int Oggetto::getHeight()
{
    return _height;
}

TipoDiOggetto Oggetto::getTipoDiOggetto()
{
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
        aggiungiPuntoAFigura(&_figura, _x + 1, _y, " ");
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
        aggiungiPuntoAFigura(&_figura, _x + 1, _y, " ");
        _clock = 100000;
        _solido = true;
        break;
    case OS_PIATTAFORMA:
        aggiungiPuntoAFigura(&_figura, _x, _y, "=");
        aggiungiPuntoAFigura(&_figura, _x + 1, _y, "=");
        aggiungiPuntoAFigura(&_figura, _x + 2, _y, "=");
        _clock = 100000;
        _solido = false;
        break;
    case OS_PIATTAFORMA2:
        aggiungiPuntoAFigura(&_figura, _x, _y, "=");
        aggiungiPuntoAFigura(&_figura, _x + 1, _y, "=");
        aggiungiPuntoAFigura(&_figura, _x + 2, _y, "=");
        aggiungiPuntoAFigura(&_figura, _x + 3, _y, "=");
        aggiungiPuntoAFigura(&_figura, _x + 4, _y, "=");
        _clock = 100000;
        _solido = false;
        break;
    case OS_WINDMILL:
        caricaFigura("WINDMILL", asciiArt);
        _clock = 100000;
        _solido = true;
        break;
    }
    calcWidthAndHeight();
}

void Oggetto::caricaFigura(char *nome, ConvertiAsciiArt *asciiArt)
{
    if (_figura != NULL)
        delete _figura;
    _figura = NULL;
    figura t = asciiArt->getFigura(nome);
    while (t != NULL)
    {
        aggiungiPuntoAFigura(&_figura, t->x + _x, t->y + _y, t->c);
        t = t->next;
    }
}

figura Oggetto::ottieniFigura()
{
    return _figura;
}

void Oggetto::calcWidthAndHeight()
{
    if (_figura != NULL)
    {
        int min_w = _figura->x;
        int max_w = _figura->x;
        int min_h = _figura->y;
        int max_h = _figura->y;
        figura tmp = _figura;
        while (tmp != NULL)
        {
            if (tmp->x > max_w)
            {
                max_w = tmp->x;
            }
            if (tmp->x < min_w)
            {
                max_w = tmp->x;
            }
            if (tmp->y > max_h)
            {
                max_h = tmp->y;
            }
            if (tmp->y < min_h)
            {
                max_h = tmp->y;
            }

            tmp = tmp->next;
        }

        _width = max_w - min_w;
        _height = max_h - min_h;
    }
}

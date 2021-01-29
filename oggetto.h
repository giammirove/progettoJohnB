#ifndef OGGETTO_H_INCLUDED
#define OGGETTO_H_INCLUDED

#include <string.h>
#include "convertiAsciiArt.h"
#include "utility.h"

enum TipoDiOggetto
{
    OS_PIATTAFORMA,
    OS_PIATTAFORMA2,
    OS_ALBERO,
    OS_CESPUGLIO,
    OS_KIRBY,
    OS_WINDMILL
};

class Oggetto
{

public:
    static int _id;

    Oggetto(int x, int y, TipoDiOggetto tipo, ConvertiAsciiArt *asciiArt);
    int getId();
    int getClock();
    bool getSolido();
    int getWidth();
    int getHeight();
    figura ottieniFigura();
    TipoDiOggetto getTipoDiOggetto();

private:
    int _x, _y;
    int _width, _height;
    int _myId;
    int _clock;
    bool _solido;
    figura _figura;
    TipoDiOggetto _tipo;

    void impostaFigura(TipoDiOggetto tipo, ConvertiAsciiArt *asciiArt);
    void caricaFigura(char *nome, ConvertiAsciiArt *asciiArt);
    void calcWidthAndHeight();
};

#endif
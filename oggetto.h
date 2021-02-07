#ifndef OGGETTO_H_INCLUDED
#define OGGETTO_H_INCLUDED

#include <string.h>
#include "convertiAsciiArt.h"
#include "utility.h"

/*
    LISTA DEI TIPI DI OGGETTO
    si tratta di un enum, ovvero un tipo di dato
    dove ad ogni valore associato un numero naturale
    es. OS_PIATTAFORMA = 0, OS_PIATTAFORMA2 = 1

    I PRIMI N OGGETTI (AL MOMENTO 2) 
    sono utilizzati per la generazione delle piattaforme
*/
enum TipoDiOggetto
{
    OS_PIATTAFORMA,
    OS_PIATTAFORMA2,
    OS_PIATTAFORMA3,
    OS_PIATTAFORMA4,
    OS_PIATTAFORMA5,
    OS_PIATTAFORMA6,
    OS_PIATTAFORMA7,
    OS_PIATTAFORMA8,
    OS_PIATTAFORMA9,
    OS_PIATTAFORMA10,
    OS_NEMICO1,
    OS_NEMICO2,
    OS_NEMICO3,
    OS_NEMICO4,
    OS_NEMICO5,
    OS_NEMICO6,
    OS_NEMICO7,
    OS_NEMICO8,
    OS_NEMICO9,
    OS_NEMICO10,
    OS_ARMA1,
    OS_ARMA1_DX,
    OS_ARMA1_SX,
    OS_ALBERO,
    OS_CESPUGLIO,
    OS_KIRBY,
    OS_WINDMILL,
    OS_PAVIMENTO
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
    figura getFigura();
    figura muoviFigura(int inc_x, int inc_y);
    TipoDiOggetto getTipoDiOggetto();

protected:
    int _x, _y;
    int _width, _height;
    int _myId;
    int _clock;
    bool _solido;
    figura _figura;
    TipoDiOggetto _tipo;

    void impostaFigura(TipoDiOggetto tipo, ConvertiAsciiArt *asciiArt);
    void caricaFigura(figura *fig, char *nome, ConvertiAsciiArt *asciiArt);
    void caricaFigura(figura *fig, figura src);
    void calcWidthAndHeight();
};

#endif
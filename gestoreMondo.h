#ifndef GESTOREMONDO_H_INCLUDED
#define GESTOREMONDO_H_INCLUDED

#include <string.h>
#include "oggetto.h"
#include "convertiAsciiArt.h"
#include "nemico.h"

class GestoreMondo
{
private:
    // grandezza della fascia dove compare la piattaforma
    int _width;
    int _map_h;
    int _max_h;
    int _min_h;
    int _saltoPlayer;
    int _altezzaPlayer;
    int _numeroPiattaforme;
    int _numeroNemici;
    int _lastXPavimento;

    ConvertiAsciiArt *_asciiArt;
    Oggetto *_oggettoPrec;
    Nemico *_nemicoPrec;

public:
    GestoreMondo(ConvertiAsciiArt *asciiArt);
    GestoreMondo(int _w, int map_h, int max_h, int min_h, int saltoPlayer, int altezzaPlayer, int numeroPiattaforme, int numeroNemici, ConvertiAsciiArt *asciiArt);
    Oggetto *generaOggetto();
    Nemico *generaNemico();
    bool generoPavimento(int score);
    bool necessitoDiGenerareOggetto(int x);
    int getXPavimento();
};

#endif
#ifndef GESTOREMONDO_H_INCLUDED
#define GESTOREMONDO_H_INCLUDED

#include <string.h>
#include "oggetto.h"
#include "convertiAsciiArt.h"

class GestoreMondo {
    private :
        // grandezza della fascia dove compare la piattaforma
        int _width;
        int _map_h;
        int _max_h;
        int _min_h;
        int _saltoPlayer;
        ConvertiAsciiArt *_asciiArt;
        Oggetto *_oggettoPrec;

    public:
        GestoreMondo (ConvertiAsciiArt *asciiArt);
        GestoreMondo (int _w, int map_h, int max_h, int min_h, int saltoPlayer, ConvertiAsciiArt *asciiArt);
        Oggetto *generaOggetto ();
};

#endif
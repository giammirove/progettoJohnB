#ifndef WEAPON
#define WEAPON

#include <string.h>
#include "oggetto.h"
#include "map.h"
#include "utility.h"

class Weapon : public Oggetto
{
private:
    int _danno;
    bool _direzione; // true sse sinistra
    figura _figuraDx, _figuraSx;

public:
    Weapon(int x, int y, TipoDiOggetto tipo, ConvertiAsciiArt *asciiArt);
    bool hit(Map *mappa);

    figura getFigura();
    figura muoviFigura(int inc_x, int inc_y);
    void impostaFigura(TipoDiOggetto tipo, ConvertiAsciiArt *asciiArt);

    bool getDirezione();
    bool setDirezione(bool val);
};

#endif
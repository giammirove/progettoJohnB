#ifndef WEAPON
#define WEAPON

#include <string.h>
#include "oggetto.h"
#include "map.h"

class Weapon : public Oggetto
{
private:
    int _danno;
    Map *_mappa;
    figura _figura;

public:
    Weapon(int x, int y, TipoDiOggetto tipo, ConvertiAsciiArt *asciiArt, Map *mappa, int danno);
    bool hit(Map *mappa);
};

#endif
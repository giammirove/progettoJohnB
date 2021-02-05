#include "weapon.h"


Weapon::Weapon(int x, int y, TipoDiOggetto tipo, ConvertiAsciiArt *asciiArt, Map *mappa, int danno):Oggetto(x, y, tipo, asciiArt)
{
    _danno = danno;
    _mappa = mappa;
}

bool Weapon::hit(Map *mappa)
{
    int buff_id = mappa->controllaCollisione(ottieniFigura());
    return true;
}
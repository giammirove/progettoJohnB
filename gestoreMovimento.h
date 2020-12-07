#ifndef GESTOREMOVIMENTO_H_INCLUDED
#define GESTOREMOVIMENTO_H_INCLUDED

#include "player.h"
#include "map.h"
#include "utility.h"
#include "listaOggetto.h"
#include <ncurses.h>

namespace GestoreMovimento
{

    namespace
    {
        bool possoLaterale(Map *map, Player *player, ListaOggetto *listaObj, int val);
        bool possoVerticale(Map *map, Player *player, ListaOggetto *listaObj, int val);
    } // namespace

    bool possoSinistra(Map *map, Player *player, ListaOggetto *listaObj);
    bool possoDestra(Map *map, Player *player, ListaOggetto *listaObj);
    bool possoSu(Map *map, Player *player, ListaOggetto *listaObj);
    bool possoGiu(Map *map, Player *player, ListaOggetto *listaObj);
} // namespace GestoreMovimento

#endif
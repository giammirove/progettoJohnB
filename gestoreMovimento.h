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
        bool possoLaterale(Map *map, figura fig, ListaOggetto *listaObj, int val);
        bool possoVerticale(Map *map, figura fig, ListaOggetto *listaObj, int val);
        bool possoMovimento(Map *map, figura fig, ListaOggetto *listaObj, int val_x, int val_y);
    } // namespace

    bool possoSinistra(Map *map, Player *player, ListaOggetto *listaObj);
    bool possoDestra(Map *map, Player *player, ListaOggetto *listaObj);
    bool possoSu(Map *map, Player *player, ListaOggetto *listaObj);
    bool possoGiu(Map *map, Player *player, ListaOggetto *listaObj);

    bool possoSinistra(Map *map, figura fig, ListaOggetto *listaObj);
    bool possoDestra(Map *map, figura fig, ListaOggetto *listaObj);
    bool possoSu(Map *map, figura fig, ListaOggetto *listaObj);
    bool possoGiu(Map *map, figura fig, ListaOggetto *listaObj);
} // namespace GestoreMovimento

#endif
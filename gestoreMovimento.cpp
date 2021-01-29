#include "gestoreMovimento.h"

namespace GestoreMovimento
{

    namespace
    {
        bool possoLaterale(Map *map, Player *player, ListaOggetto *listaObj, int val)
        {
            // val = -1 -> sinistra
            // val = 1 -> destra
            int id_coll = map->controllaCollisione(player->getFigura(), val, 0);
            if ((map->dentroMargine(player->getFigura(), val, 0)))
            {
                bool solido = (id_coll != -1) ? listaObj->getDaId(id_coll).getSolido() : solido = false;
                if (id_coll == -1 || (id_coll != -1 && solido == false))
                {
                    return true;
                }else {
                    return false;
                }
            }
            return false;
        }
        bool possoVerticale(Map *map, Player *player, ListaOggetto *listaObj, int val)
        {
            // val = -1 -> alto
            // val = 1 -> giu
            int id_coll = map->controllaCollisione(player->getFigura(), 0, val);
            if ((map->dentroMargine(player->getFigura(), 0, val)))
            {
                bool solido = (id_coll != -1) ? listaObj->getDaId(id_coll).getSolido() : solido = false;
                // se scendo me ne frego se è solido o no
                if (val == 1) {
                    solido = true;
                    id_coll = map->controllaCollisionePiattaforme(player->getFigura(), 0, val);
                }
                if (id_coll == -1 || (id_coll != -1 && solido == false))
                {
                    return true;
                }else {
                    return false;
                }
            }
            return false;
        }
    } // namespace

    bool possoSinistra(Map *map, Player *player, ListaOggetto *listaObj)
    {
        return possoLaterale(map, player, listaObj, -1);
    }
    bool possoDestra(Map *map, Player *player, ListaOggetto *listaObj)
    {
        return possoLaterale(map, player, listaObj, 1);
    }
    bool possoSu(Map *map, Player *player, ListaOggetto *listaObj) {
        return possoVerticale(map, player, listaObj, -1);
    }
    bool possoGiu(Map *map, Player *player, ListaOggetto *listaObj) {
        return possoVerticale(map, player, listaObj, 1);
    }
} // namespace GestoreMovimento
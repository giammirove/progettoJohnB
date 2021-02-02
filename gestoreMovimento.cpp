#include "gestoreMovimento.h"

namespace GestoreMovimento
{

    namespace
    {
        /*
            Queste funzioni si possono rifare anche per la classe Oggetto
            in alternativa alla classe Player per permettere la gestione
            dei nemici o entità
        */

        /*
            PossoLaterale verifica se il player può muoversi sull'asse orizzontale
            map = la mappa del gioco
            player = oggetto del player
            listaObj = è lista degli oggetti nel gioco
            val = indica di quanto bisogna modificare la x per verificare il movimento
        */
        bool possoLaterale(Map *map, Player *player, ListaOggetto *listaObj, int val)
        {
            // val = -1 -> sinistra
            // val = 1 -> destra
            // verifico le collisioni del player rispetto alla sua x incrementata di val
            // id_coll è uguale all'id dell'entità con cui il player collide
            int id_coll = map->controllaCollisione(player->getFigura(), val, 0);
            // se il player si trova all'interno del margine verifico le collisioni
            if ((map->dentroMargine(player->getFigura(), val, 0)))
            {
                // se id_coll != -1 (ovvero collide) allora solido è determinato dall'oggetto 
                // con cui collido, altrimenti non mi interessa e solido è false di default
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

        /*  
            PossoVerticale verifica se il player può muoversi sull'asse orizzontale
            guardare la documentazione di PossoOrizzontale, sono quasi uguali
        */
        bool possoVerticale(Map *map, Player *player, ListaOggetto *listaObj, int val)
        {
            // val = -1 -> alto
            // val = 1 -> giu
            int id_coll = map->controllaCollisione(player->getFigura(), 0, val);
            if ((map->dentroMargine(player->getFigura(), 0, val)))
            {
                bool solido = (id_coll != -1) ? listaObj->getDaId(id_coll).getSolido() : solido = false;
                // se scendo me ne frego se è solido o no
                // val = 1 sse sto scendendo, mentre scendo tutti gli oggetti sono considerati come solidi
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
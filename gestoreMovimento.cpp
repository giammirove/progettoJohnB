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
        bool possoLaterale(Map *map, figura fig, ListaOggetto *listaObj, int val)
        {
            // val = -1 -> sinistra
            // val = 1 -> destra
            // verifico le collisioni del player rispetto alla sua x incrementata di val
            // id_coll è uguale all'id dell'entità con cui il player collide
            riga coll = map->datiCollisione(fig, val, 0);
            int id_coll = coll->id;
            bool solido_coll = coll->solido;
            // se il player si trova all'interno del margine verifico le collisioni
            if ((map->dentroMargine(fig, val, 0)))
            {
                // se id_coll != -1 (ovvero collide) allora solido è determinato dall'oggetto
                // con cui collido, altrimenti non mi interessa e solido è false di default
                bool solido = (id_coll != -1) ? solido_coll : solido = false;
                if (id_coll == -1 || (id_coll != -1 && solido == false))
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            return false;
        }

        /*  
            PossoVerticale verifica se il player può muoversi sull'asse orizzontale
            guardare la documentazione di PossoOrizzontale, sono quasi uguali
        */
        bool possoVerticale(Map *map, figura fig, ListaOggetto *listaObj, int val)
        {
            // val = -1 -> alto
            // val = 1 -> giu
            riga coll = map->datiCollisione(fig, 0, val);
            int id_coll = coll->id;
            bool solido_coll = coll->solido;
            if ((map->dentroMargine(fig, 0, val)))
            {
                bool solido = (id_coll != -1) ? solido_coll : solido = false;
                // se scendo me ne frego se è solido o no
                // val = 1 sse sto scendendo, mentre scendo tutti gli oggetti sono considerati come solidi
                if (val == 1)
                {
                    solido = true;
                    id_coll = map->controllaCollisionePiattaforme(fig, 0, val);
                }
                if (id_coll == -1 || (id_coll != -1 && solido == false))
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            return false;
        }

        /*
            Verifico se puo effettuare un movimento in base ai parametri val_x e val_y
        */
        bool possoMovimento(Map *map, figura fig, ListaOggetto *listaObj, int val_x, int val_y)
        {
            // val = -1->sinistra
            // val = 1 -> destra
            // verifico le collisioni del player rispetto alla sua x incrementata di val
            // id_coll è uguale all'id dell'entità con cui il player collide
            riga coll = map->datiCollisione(fig, val_x, val_y);
            int id_coll = coll->id;
            bool solido_coll = coll->solido;
            // se il player si trova all'interno del margine verifico le collisioni
            if ((map->dentroMargine(fig, val_x, val_y)))
            {
                // se id_coll != -1 (ovvero collide) allora solido è determinato dall'oggetto
                // con cui collido, altrimenti non mi interessa e solido è false di default
                bool solido = (id_coll != -1) ? solido_coll : solido = false;
                if (id_coll == -1 || (id_coll != -1 && solido == false))
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            return false;
        }
    } // namespace

    bool possoSinistra(Map *map, Player *player, ListaOggetto *listaObj)
    {
        return possoLaterale(map, player->getFigura(), listaObj, -1);
    }
    bool possoDestra(Map *map, Player *player, ListaOggetto *listaObj)
    {
        return possoLaterale(map, player->getFigura(), listaObj, 1);
    }
    bool possoSu(Map *map, Player *player, ListaOggetto *listaObj)
    {
        return possoVerticale(map, player->getFigura(), listaObj, -1);
    }
    bool possoGiu(Map *map, Player *player, ListaOggetto *listaObj)
    {
        return possoVerticale(map, player->getFigura(), listaObj, 1);
    }

    bool possoSinistra(Map *map, figura fig, ListaOggetto *listaObj)
    {
        return possoLaterale(map, fig, listaObj, -1);
    }
    bool possoDestra(Map *map, figura fig, ListaOggetto *listaObj)
    {
        return possoLaterale(map, fig, listaObj, 1);
    }
    bool possoSu(Map *map, figura fig, ListaOggetto *listaObj)
    {
        return possoVerticale(map, fig, listaObj, -1);
    }
    bool possoGiu(Map *map, figura fig, ListaOggetto *listaObj)
    {
        return possoVerticale(map, fig, listaObj, 1);
    }
} // namespace GestoreMovimento
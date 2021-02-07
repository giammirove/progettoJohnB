#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include <unistd.h>
#include <typeinfo>
#include "time.h"
#include <fstream>
#include <iostream>
#include "gioco.h"
#include "map.h"
#include "player.h"
#include "oggetto.h"
#include "listaOggetto.h"
#include "convertiAsciiArt.h"
#include "gestoreMovimento.h"
#include "gestoreMondo.h"
#include "listaNemici.h"
#include "nemico.h"

// PUBLIC

Gioco::Gioco(FILE *read)
{
    asciiArt = new ConvertiAsciiArt(read);

    player = new Player(0, 0, 20, 12, asciiArt->getFigura("PG"));
    player->setArma(OS_ARMA1, asciiArt);
    map = new Map(W_WIN - 2, H_WIN - 2);

    gestoreMondo = new GestoreMondo(6, map->getHeight(), map->getHeight(), 6, 12, NUM_PIATTAFORME, NUM_NEMICI, asciiArt);

    listaObj = new ListaOggetto();
    listaNem = new ListaNemici();
    
    // Genera di default prima 15 stutture
    for (int i = 0; i < 15; i++)
    {
        aggiungiBloccoAlMondo();
        if (i % 2 == 0)
        {
            aggiungiNemico();
        }
        map->spostaVistaDestra();
    }
    for (int i = 0; i < 15; i++)
    {
        map->spostaVistaSinistra();
    }
}

/*
    Ritorna l'oggetto Player
*/
Player *Gioco::getPlayer()
{
    return player;
}

/*
    Ritorna l'oggetto Converti Ascii Art
*/
ConvertiAsciiArt *Gioco::getAsciiArt()
{
    return asciiArt;
}

/*
    Ritorna l'oggetto gestore mondo
*/
GestoreMondo *Gioco::getGestoreMondo()
{
    return gestoreMondo;
}

/*
    Ritorna l'oggetto mappa
*/
Map *Gioco::getMap()
{
    return map;
}

/*
    Ritorna l'oggetto della lista di oggetti
*/
ListaOggetto *Gioco::getListaObj()
{
    return listaObj;
}

/*
    Ritorna l'oggetto della lista di nemici
*/
ListaNemici *Gioco::getListaNem()
{
    return listaNem;
}

/*
    Gestisce tutto il gioco 
*/
void Gioco::gestisciGioco(int c, int *prev, int sec, int nemClock, bool *aggiorna)
{
    // Gestisce i nemici
    gestioneNemici(nemClock, aggiorna);

    gestioneGravitaESalto(sec, c, prev, aggiorna);

    // Gestisco gli input da tastiera
    elaboraInput(c, prev);
}

// PRIVATE

/*
    Funzione che gestisce gli input (c) e agisce di conseguenza
*/
void Gioco::elaboraInput(int c, int *prev)
{

    /*
        EVENTO FRECCIA IN ALTO PREMUTA
    */
    if (c == KEY_UP && player->getATerra())
    {
        // Verifico se posso andare verso l'alto
        if (GestoreMovimento::possoSu(map, player, listaObj))
        {
            // In caso affermativo salto
            player->salta();

            // Se l'input precedente è stato FRECCIA DESTRA allora effettuo un salto a destra
            if (*prev == KEY_RIGHT)
            {
                player->resettaSaltoDestraSinistra();
                player->saltaDestra();
                // cambio la figura del player in posizione di destra
                player->setFigura(asciiArt->getFigura("PG_DX"));
            }
            // Se l'input precedente è stato FRECCIA SINISTRA allora effettuo un salto a sinistra
            else if (*prev == KEY_LEFT)
            {
                player->resettaSaltoDestraSinistra();
                player->saltaSinistra();
                // cambio la figura del player in posizione di sinistra
                player->setFigura(asciiArt->getFigura("PG_SX"));
            }
            // Altrimenti effettuo un semplice salto
            else
            {
                // cambio la figura del player in posizione idle
                player->setFigura(asciiArt->getFigura("PG"));
                *prev = c;
            }
        }
    }

    /*
        EVENTO FRECCIA SINISTRA PREMUTA
    */
    if (c == KEY_LEFT)
    {
        if (player->getSaltaInt() > 0)
        {
            player->resettaSaltoDestraSinistra();
            player->saltaSinistra();
        }
        if ((player->getATerra() || !(player->getSaltaDestra() || player->getSaltaSinistra())))
        {
            if (player->getATerra())
            {
                // Verifico se posso effettuare un movimento a sinistra
                int possoSinistra = GestoreMovimento::possoSinistra(map, player, listaObj);
                // Verifico se è possibile spostare la vista a sinistra
                if (map->possoSpostareVistaSinistra(listaObj, player->getFigura(), player->getView()))
                {
                    // In caso affermativo sposto la vista a sinistra
                    map->spostaVistaSinistra();

                    // Se l'input precedente è stato FRECCIA IN ALTO allora effettuo un salto a sinistra
                    if (*prev == KEY_UP)
                    {
                        player->resettaSaltoDestraSinistra();
                        player->saltaSinistra();
                    }

                    // Verifico di essere arrivato al limite sinistro della mappa
                    if (map->getOffset() == 0 && possoSinistra)
                    {
                        // In caso mi trovi nella prima vista della mappa e possa muovermi a sinistra
                        player->vaiASinistra();
                    }
                    // cambio la figura del player in posizione di sinistra
                    player->setFigura(asciiArt->getFigura("PG_SX"));
                    *prev = c;
                }
                // Se non è possibile spostare la vista a sinistra, ma invece è possibile effettuare un movimento
                else if (possoSinistra)
                {
                    // Se l'input precedente è stato FRECCIA IN ALTO allora effettuo un salto a sinistra
                    if (*prev == KEY_UP)
                    {
                        player->resettaSaltoDestraSinistra();
                        player->saltaSinistra();
                    }
                    // Altrimenti effettuo un semplice movimento a sinistra
                    else
                    {
                        player->vaiASinistra();
                    }
                    // cambio la figura del player in posizione di sinistra
                    player->setFigura(asciiArt->getFigura("PG_SX"));
                    *prev = c;
                }
            }
            else if (*prev == KEY_UP)
            {
                // Verifico se posso effettuare un movimento a sinistra
                int possoSinistra = GestoreMovimento::possoSinistra(map, player, listaObj);
                // Verifico se è possibile spostare la vista a sinistra
                if (map->possoSpostareVistaSinistra(listaObj, player->getFigura(), player->getView()))
                {
                    // In caso affermativo sposto la vista a sinistra
                    map->spostaVistaSinistra();
                    aggiungiBloccoAlMondo();

                    // Se l'input precedente è stato FRECCIA IN ALTO allora effettuo un salto a sinistra
                    player->resettaSaltoDestraSinistra();
                    player->saltaSinistra();
                    // cambio la figura del player in posizione di sinistra
                    player->setFigura(asciiArt->getFigura("PG_SX"));
                }
                // Se non è possibile spostare la vista a sinistra, ma invece è possibile effettuare un movimento
                else if (possoSinistra)
                {
                    // Se l'input precedente è stato FRECCIA IN ALTO allora effettuo un salto a sinistra
                    player->resettaSaltoDestraSinistra();
                    player->saltaSinistra();
                    // cambio la figura del player in posizione di sinistra
                    player->setFigura(asciiArt->getFigura("PG_SX"));
                }
                *prev = c;
            }
        }
    }

    /*
        EVENTO FRECCIA DESTRA PREMUTA
    */
    if (c == KEY_RIGHT)
    {
        if (player->getSaltaInt() > 0)
        {
            player->resettaSaltoDestraSinistra();
            player->saltaDestra();
        }
        if ((player->getATerra() || !(player->getSaltaDestra() || player->getSaltaSinistra())))
        {
            if (player->getATerra())
            {
                // Verifico se posso effettuare un movimento a destra
                int possoDestra = GestoreMovimento::possoDestra(map, player, listaObj);
                // Verifico se è possibile spostare la vista a destra
                if (map->possoSpostareVistaDestra(listaObj, player->getFigura(), player->getView()))
                {
                    // In caso affermativo sposto la vista a destra
                    map->spostaVistaDestra();
                    aggiungiBloccoAlMondo();

                    // Se l'input precedente è stato FRECCIA IN ALTO allora effettuo un salto a destra
                    if (*prev == KEY_UP)
                    {
                        player->resettaSaltoDestraSinistra();
                        player->saltaDestra();
                    }
                    // cambio la figura del player in posizione di destra
                    player->setFigura(asciiArt->getFigura("PG_DX"));
                    *prev = c;
                }
                // Se non è possibile spostare la vista a sinistra, ma invece è possibile effettuare un movimento
                else if (possoDestra)
                {
                    // Se l'input precedente è stato FRECCIA IN ALTO allora effettuo un salto a sinistra
                    if (*prev == KEY_UP)
                    {
                        player->resettaSaltoDestraSinistra();
                        player->saltaDestra();
                    }
                    // Altrimenti effettuo un semplice movimento a sinistra
                    else
                    {
                        player->vaiADestra();
                    }
                    // cambio la figura del player in posizione di destra
                    player->setFigura(asciiArt->getFigura("PG_DX"));
                    *prev = c;
                }
            }
            else if (*prev == KEY_UP)
            {
                // Verifico se posso effettuare un movimento a destra
                int possoDestra = GestoreMovimento::possoDestra(map, player, listaObj);
                // Verifico se è possibile spostare la vista a destra
                if (map->possoSpostareVistaDestra(listaObj, player->getFigura(), player->getView()))
                {
                    // In caso affermativo sposto la vista a destra
                    map->spostaVistaDestra();
                    aggiungiBloccoAlMondo();

                    // Se l'input precedente è stato FRECCIA IN ALTO allora effettuo un salto a destra
                    player->resettaSaltoDestraSinistra();
                    player->saltaDestra();
                    // cambio la figura del player in posizione di destra
                    player->setFigura(asciiArt->getFigura("PG_DX"));
                }
                // Se non è possibile spostare la vista a sinistra, ma invece è possibile effettuare un movimento
                else if (possoDestra)
                {
                    // Se l'input precedente è stato FRECCIA IN ALTO allora effettuo un salto a sinistra
                    player->resettaSaltoDestraSinistra();
                    player->saltaDestra();
                    // cambio la figura del player in posizione di destra
                    player->setFigura(asciiArt->getFigura("PG_DX"));
                }
                *prev = c;
            }
        }
    }

#pragma region DEBUG
    /*
        EVENTO T PREMUTA
        Aggiungo una piattaforma 
    */
    if (c == 't')
    {
        aggiungiOggetto(new Oggetto(player->getX() + player->getWidth() + map->getOffset() + 1, player->getY(), OS_PIATTAFORMA, asciiArt));
    }
    /*
        EVENTO R PREMUTA
        Aggiungo un mulino a vento 
    */
    if (c == 'r')
    {
        aggiungiOggetto(new Oggetto(player->getX() + player->getWidth() + map->getOffset() + 1, player->getY(), OS_WINDMILL, asciiArt));
    }
    /*
        EVENTO SPAZIONE PREMUTA
        Elimino un elemento alla mia destra
    */
    if (c == ' ')
    {
        int id = map->controllaCollisionePiattaforme(player->getFigura(), 1, 0);
        if (id != -1)
        {
            map->rimuoviOggetto(listaObj->getDaId(id));
            listaObj->rimuoviDaId(id);
        }
    }
    /*
        EVENTO K PREMUTA
        Aggiungo un oggetto casuale 
    */
    if (c == 'k')
    {
        Oggetto *tmp = gestoreMondo->generaOggetto();
        aggiungiOggetto(tmp);
    }
    /*
        EVENTO P PREMUTA
        Mi trasformo in un rinoceronte
    */
    if (c == 'p')
    {
        player->setFigura(asciiArt->getFigura("RINO"));
    }
    /*
        EVENTO O PREMUTA
        Aggiungo un pipistrello 
    */
    if (c == 'o')
    {
        player->setFigura(asciiArt->getFigura("BAT"));
    }
    /*
        EVENTO I PREMUTA
        Aggiungo una rana 
    */
    if (c == 'i')
    {
        player->setFigura(asciiArt->getFigura("FROG"));
    }
    if (c == 'd')
    {
        listaNemico lista = listaNem->getListaNemico();
        if (map->nemicoDentroMargine(lista->nem->getFigura()))
        {
            map->rimuoviOggetto((lista->nem));
            lista->nem->muoviNemico(map);
            map->aggiungiOggetto((lista->nem));
        }
    }
    if (c == 'c')
    {
        player->cambiaArmaAttiva();
    }

#pragma endregion
}

/*
    Gestisci la collisione del player con i nemici e con le armi
*/
void Gioco::gestioneCollisioneNemiciEArmi()
{
    if (player->getArmaAttiva())
    {
        int id_coll = map->controllaCollisione(player->getArma()->getFigura());
        if (id_coll != -1)
        {
            if (listaNem->getDaId(id_coll) != NULL)
            {
                mvprintw(4, 100, "NEMICO TOCCATO %d", id_coll);
                // Elimina il nemico
                map->rimuoviOggetto(listaNem->getDaId(id_coll));
                listaObj->rimuoviDaId(id_coll);
                listaNem->rimuoviDaId(id_coll);
            }
        }
    }

    int id_coll = map->controllaCollisione(player->getFigura());
    if (id_coll != -1)
    {
        Nemico *nem = listaNem->getDaId(id_coll);
        if (nem != NULL)
        {
            player->decrementaVita(nem->getAttacco());
            // Elimina il nemico
            map->rimuoviOggetto(listaNem->getDaId(id_coll));
            listaObj->rimuoviDaId(id_coll);
            listaNem->rimuoviDaId(id_coll);
        }
    }
}

/*
    Funzione che gestise la gravità del player
    Il player effettua un salto a cono con la punta tagliata
      @@
     @  @
    @    @   idea generale
*/
void Gioco::gestioneGravitaESalto(int sec, int c, int *prev, bool *aggiorna)
{

    // Questo controllo permette di temporizzare la gravità
    if (sec % player->getClock() == 0)
    {
        // Qui sta continuando il salto
        if (player->stoSaltando())
        {
            // Verifico se il player può effettuare un movimento verso il basso
            if (GestoreMovimento::possoSu(map, player, listaObj))
            {
                // Se il player sta effettuando un salto a destra
                if (player->getSaltaDestra())
                {
                    // Verifico se il player può effettuare un movimento a destra
                    if (GestoreMovimento::possoDestra(map, player, listaObj))
                    {
                        // Verifico se è possibile spostare la vista a destra
                        if (map->possoSpostareVistaDestra(listaObj, player->getFigura(), player->getView()))
                        {
                            // In caso affermativo la sposto
                            map->spostaVistaDestra();
                            aggiungiBloccoAlMondo();
                        }
                        // Altrimenti mi sposto solamente a destra
                        else
                        {
                            player->vaiADestra();
                        }
                    }
                    // Se non posso andare a destra interrompo il mio salto
                    else
                    {
                        player->resettaSalto();
                        //*prev = -1;
                    }
                }

                // Se il player sta effettuando un salto a sinistra
                if (player->getSaltaSinistra())
                {
                    // Verifico se il player può effettuare un movimento a sinistra
                    if (GestoreMovimento::possoSinistra(map, player, listaObj))
                    {
                        // Verifico se è possibile spostare la vista a sinistra
                        if (map->possoSpostareVistaSinistra(listaObj, player->getFigura(), player->getView()))
                        {
                            // In caso affermativo la sposto
                            map->spostaVistaSinistra();
                        }
                        // Altrimenti mi sposto solamente a sinistra
                        else
                        {
                            player->vaiASinistra();
                        }
                    }
                    // Se non posso andare a sinistra interrompo il mio salto
                    else
                    {
                        player->resettaSalto();
                        //*prev = -1;
                    }
                }

                // Effetuo il salto
                player->vaiInAlto();
                // Decremento il valore attuale del salto
                player->decrementaSalto();

                // E' avvenuta una modifica allo schermo quindi è necessario un refresh
                *aggiorna = true;
            }
            // Altrimenti interrompo il salto
            else
            {
                player->resettaSalto();
                //*prev = -1;
            }
        }

        // quando arriva in alto si sposta di uno a destra o sinistra
        else if (player->sonoInAriaDuranteIlSalto())
        {
            // Se il player sta effettuando un salto a destra
            if (player->getSaltaDestra())
            {
                // Verifico se il player può effettuare un movimento verso destra
                if (GestoreMovimento::possoDestra(map, player, listaObj))
                {
                    // Verifico se è possibile spostare la vista a destra
                    if (map->possoSpostareVistaDestra(listaObj, player->getFigura(), player->getView()))
                    {
                        // In caso affermativo la sposto
                        map->spostaVistaDestra();
                        aggiungiBloccoAlMondo();
                    }
                    // Altrimenti mi sposto solamente a destra
                    else
                    {
                        player->vaiADestra();
                    }
                }
                // Se non posso andare a destra interrompo il mio salto
                else
                {
                    player->resettaSalto();
                    //*prev = -1;
                }
            }

            // Se il player sta effettuando un salto a sinistra
            if (player->getSaltaSinistra())
            {
                // Verifico se il player può effettuare un movimento a sinistra
                if (GestoreMovimento::possoSinistra(map, player, listaObj))
                {
                    // Verifico se è possibile spostare la vista a sinistra
                    if (map->possoSpostareVistaSinistra(listaObj, player->getFigura(), player->getView()))
                    {
                        // In caso affermativo la sposto
                        map->spostaVistaSinistra();
                    }
                    // Altrimenti mi sposto solamente a sinistra
                    else
                    {
                        player->vaiASinistra();
                    }
                }
                // Se non posso andare a sinistra interrompo il mio salto
                else
                {
                    player->resettaSalto();
                    //*prev = -1;
                }
            }

            // Decremento il valore attuale del salto
            player->decrementaSalto();
            // E' avvenuta una modifica allo schermo quindi è necessario un refresh
            *aggiorna = true;
        }

        // Controllo se posso scendere
        else if (player->stoScendendo())
        {
            // Verifico se il player può effettuare un movimento verso il basso
            if (player->getATerra() == false)
            {
                // Effetuo un movimento verso il basso
                player->vaiInBasso();
                // controllo se sto ancora scendendo da dx o sxs
                if (player->getSaltaInt() > 0)
                {
                    // Se il player sta effettuando un salto a destra
                    if (player->getSaltaDestra())
                    {
                        // Verifico se il player può effettuare un movimento verso destra
                        if (GestoreMovimento::possoDestra(map, player, listaObj))
                        {
                            // Verifico se è possibile spostare la vista a destra
                            if (map->possoSpostareVistaDestra(listaObj, player->getFigura(), player->getView()))
                            {
                                // In caso affermativo la sposto
                                map->spostaVistaDestra();
                                aggiungiBloccoAlMondo();
                            }
                            // Altrimenti mi sposto solamente a destra
                            else
                            {
                                player->vaiADestra();
                            }
                            //*prev = KEY_RIGHT;
                        }
                        // Se non posso andare a sinistra interrompo il mio salto
                        else
                        {
                            player->resettaSalto();
                            //*prev = -1;
                        }
                    }

                    // Se il player sta effettuando un salto a sinistra
                    if (player->getSaltaSinistra())
                    {
                        // Verifico se il player può effettuare un movimento a sinistra
                        if (GestoreMovimento::possoSinistra(map, player, listaObj))
                        {
                            // Verifico se è possibile spostare la vista a sinistra
                            if (map->possoSpostareVistaSinistra(listaObj, player->getFigura(), player->getView()))
                            {
                                // In caso affermativo la sposto
                                map->spostaVistaSinistra();
                            }
                            // Altrimenti mi sposto solamente a sinistra
                            else
                            {
                                player->vaiASinistra();
                            }
                            //*prev = KEY_LEFT;
                        }
                        // Se non posso andare a sinistra interrompo il mio salto
                        else
                        {
                            player->resettaSalto();
                            //*prev = -1;
                        }
                    }
                    // Decremento il valore attuale del salto
                    player->decrementaSalto();
                }
                // Altrimenti interrompo il mio salto a destra e sinistra
                else
                {
                    //*prev = -1;
                    player->resettaSaltoDestraSinistra();
                }

                // E' avvenuta una modifica allo schermo quindi è necessario un refresh
                *aggiorna = true;
            }
            // Altrimenti interrompo il mio salto
            else
            {
                player->resettaSalto();
                *prev = -1;
            }
        }
    }

    if (GestoreMovimento::possoGiu(map, player, listaObj))
    {
        mvprintw(0, 50, "POSSO GIU %d", c);
        player->setATerra(false);
    }
    else
    {
        mvprintw(0, 50, "NON POSSO GIU %d", c);
        player->setATerra(true);
    }
}

/*
    Gestisce i nemici
*/
void Gioco::gestioneNemici(int sec, bool *aggiorna)
{
    gestioneCollisioneNemiciEArmi();
    if (sec % NEMICI_CLOCK == 0)
    {
        listaNemico lista = listaNem->getListaNemico();
        while (lista != NULL)
        {
            // Verifico se il nemico è nella schermata del player
            if (map->nemicoDentroMargine(lista->nem->getFigura()))
            {
                map->rimuoviOggetto((lista->nem));
                lista->nem->muoviNemico(map);
                map->aggiungiOggetto((lista->nem));
                *aggiorna = true;
            }
            else
            {
            }
            lista = lista->next;
        }
    }
}

/*
    Aggiunge un blocco al mondo
*/
void Gioco::aggiungiBloccoAlMondo()
{   
    if (gestoreMondo->generoPavimento())
        aggiungiOggetto(new Oggetto(gestoreMondo->getXPavimento(), map->getHeight() - 2, OS_PAVIMENTO, asciiArt));
    Oggetto *tmp = gestoreMondo->generaOggetto();
    aggiungiOggetto(tmp);
    if ((player->getX() + map->getOffset()) % 7 == 0)
        player->incrementaScore();
}

/*
    Aggiungi un nemico alla lista degli oggetti e alla mappa
*/
void Gioco::aggiungiNemico()
{
    Nemico *nem = gestoreMondo->generaNemico();
    map->aggiungiOggetto(nem);
    listaNem->aggiungi(nem);
    listaObj->aggiungi(nem);
}

/*
    Aggiungi un nemico alla lista degli oggetti e alla mappa
*/
void Gioco::aggiungiNemico(Nemico *nem)
{
    map->aggiungiOggetto(nem);
    listaNem->aggiungi(nem);
    listaObj->aggiungi(nem);
}

/*
    Aggiunge un oggetto alla mappa e alla lista di oggetti
*/
void Gioco::aggiungiOggetto(Oggetto *obj)
{
    map->aggiungiOggetto(obj);
    listaObj->aggiungi(obj);
}

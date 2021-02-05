#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include <unistd.h>
#include "time.h"
#include <fstream>
#include <iostream>
#include "map.h"
#include "player.h"
#include "oggetto.h"
#include "listaOggetto.h"
#include "convertiAsciiArt.h"
#include "gestoreMovimento.h"
#include "gestoreMondo.h"

using namespace std;

const int SCREEN_CLOCK = 100000000;
const int INPUT_CLOCK = 2000000;

int H_WIN = 20;
int W_WIN = 60;

const int MOV_LATERALE_IN_ARIA = 2;

/*
    Aggiunge un oggetto alla mappa e alla lista di oggetti
*/
void aggiungiOggetto(Map *map, ListaOggetto *listaObj, Oggetto *obj)
{
    map->aggiungiOggetto(*obj);
    listaObj->aggiungi(obj);
}

/*
    Aggiunge un blocco al mondo
*/
void aggiungiBloccoAlMondo(Map *map, Player *player, ListaOggetto *listaObj, GestoreMondo *gestoreMondo)
{
    Oggetto *tmp = gestoreMondo->generaOggetto();
    aggiungiOggetto(map, listaObj, tmp);
    if ((player->getX() + map->getOffset()) % 7 == 0)
        player->incrementaScore();
}

/*
    Disegna i punti vita del player e score
*/
void disegnaScoreEVita(WINDOW *win, Player *player)
{
    for (int i = 0; i < player->getVita() * 2; i += 2)
    {
        mvwprintw(win, 0 + 1, i + 1, "♥");
    }
    mvwprintw(win, 1 + 1, 0 + 1, "Score %d", player->getScore());
}

/*
    Disegna nella finestra win, la porzione di mappa che 
    il player attualmente sta osservando, ricavata
    tramita l'offset attuale
*/
void disegnaMappa(WINDOW *win, Map *map)
{
    mappa_t m = map->getMappa();
    //printw("offset %d", map->getOffset());
    bool limite = false;
    int c = 0;
    while (m != NULL && c < map->getView())
    {
        riga r = m->r;
        while (r != NULL)
        {
            mvwprintw(win, r->y + 1, m->x - map->getOffset() + 1, r->c);
            r = r->next;
        }

        m = m->next;
        c++;
    }
}

/*
    Disegna la figura del player nella finestra win
*/
void disegnaPlayer(WINDOW *win, Player player)
{
    figura fig = player.getFigura();
    while (fig != NULL)
    {
        mvwprintw(win, fig->y + 1, fig->x + 1, fig->c);
        fig = fig->next;
    }
}

/*
    Funzione che gestisce gli input (c) e agisce di conseguenza
*/
void elaboraInput(int c, int *prev, Map *map, Player *player, ListaOggetto *listaObj, ConvertiAsciiArt *asciiArt, GestoreMondo *gestoreMondo)
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
            // Se l'input precedente è stato FRECCIA IN ALTO effettuo un salto a sinistra
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
    /*
        EVENTO FRECCIA DESTRA PREMUTA
    */
    if (c == KEY_RIGHT)
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
                aggiungiBloccoAlMondo(map, player, listaObj, gestoreMondo);

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
                aggiungiBloccoAlMondo(map, player, listaObj, gestoreMondo);

                // Se l'input precedente è stato FRECCIA IN ALTO allora effettuo un salto a destra
                player->resettaSaltoDestraSinistra();
                player->saltaDestra();
                // cambio la figura del player in posizione di destra
                player->setFigura(asciiArt->getFigura("PG_DX"));
                *prev = c;
            }
            // Se non è possibile spostare la vista a sinistra, ma invece è possibile effettuare un movimento
            else if (possoDestra)
            {
                // Se l'input precedente è stato FRECCIA IN ALTO allora effettuo un salto a sinistra
                player->resettaSaltoDestraSinistra();
                player->saltaDestra();
                // cambio la figura del player in posizione di destra
                player->setFigura(asciiArt->getFigura("PG_DX"));
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
        aggiungiOggetto(map, listaObj, new Oggetto(player->getX() + player->getWidth() + map->getOffset() + 1, player->getY(), OS_PIATTAFORMA, asciiArt));
    }
    /*
        EVENTO R PREMUTA
        Aggiungo un mulino a vento 
    */
    if (c == 'r')
    {
        aggiungiOggetto(map, listaObj, new Oggetto(player->getX() + player->getWidth() + map->getOffset() + 1, player->getY(), OS_WINDMILL, asciiArt));
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
        aggiungiOggetto(map, listaObj, tmp);
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

#pragma endregion

    // CONTROLLA COLLISIONE
    /*
    if (map->controllaCollisionePiattaforme(player->getFigura()) != -1)
    {
        int id = map->controllaCollisionePiattaforme(player->getFigura());
        int tipo = listaObj->getDaId(id).getTipoDiOggetto();
        mvprintw(0, 0, "\tCOLLISIONE : %d (x:%d) (y:%d)", id, player->getX(), player->getY());
        if (tipo == 0)
            mvprintw(0, 0, " TIPO : ALBERO");
        else if (tipo == 1)
            mvprintw(0, 0 , " TIPO : CESPUGLIO");
    }
    */
}

/*
    Funzione che gestice l'aggiornamento dello schermo nella finestra win 
*/
void aggiornaSchermo(WINDOW *win, WINDOW *debug, Map *map, Player *player)
{
    // Elimino ciò che era presente prima
    werase(win);
    werase(debug);

    // Ridisegno i bordi
    box(win, 0, 0);
    box(debug, 0, 0);
    // Disegno la mappa nella finestra win
    disegnaMappa(win, map);

    // Disegno il player nella finestra win
    disegnaPlayer(win, *player);

    // Disegno score e vita del player
    disegnaScoreEVita(win, player);

    // Aggiorno per apportare le modifiche
    refresh();
    wrefresh(win);
    wrefresh(debug);
}

/*
    Funzione che gestise la gravità del player
    Il player effettua un salto a cono con la punta tagliata
      @@
     @  @
    @    @   idea generale
*/
void gestioneGravitaESalto(int sec, int c, int *prev, bool *aggiorna, Map *map, Player *player, ListaOggetto *listaObj, GestoreMondo *gestoreMondo)
{

    // Qui sta continuando il salto
    if (player->stoSaltando())
    {
        // Questo controllo permette di temporizzare la gravità
        if (sec % player->getClock() == 0)
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
                            aggiungiBloccoAlMondo(map, player, listaObj, gestoreMondo);
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
    }

    // quando arriva in alto si sposta di uno a destra o sinistra
    else if (player->sonoInAriaDuranteIlSalto())
    {
        // Questo controllo permette di temporizzare la gravità
        if (sec % player->getClock() == 0)
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
                        aggiungiBloccoAlMondo(map, player, listaObj, gestoreMondo);
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
    }
    // Controllo se posso scendere
    else if (sec % player->getClock() == 0)
    {
        // Verifico se il player può effettuare un movimento verso il basso
        if (GestoreMovimento::possoGiu(map, player, listaObj))
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
                            aggiungiBloccoAlMondo(map, player, listaObj, gestoreMondo);
                        }
                        // Altrimenti mi sposto solamente a destra
                        else
                        {
                            player->vaiADestra();
                        }
                        *prev = KEY_RIGHT;
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
                        *prev = KEY_LEFT;
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

int main()
{
#pragma region PARAM N_CURSES

    srand(time(NULL));
    setlocale(LC_ALL, "");
    //const wchar_t *wstr = L"\u2603\u26c4\U0001F638";
    initscr();
    timeout(1);
    //initscr(); /* Start curses mode 		  */
    noecho();
    cbreak();

    // nasconde il cursore
    curs_set(0);

#pragma endregion

#pragma region INIT

    WINDOW *win = newwin(H_WIN, W_WIN, 1, 1);
    nodelay(win, true);
    scrollok(win, TRUE);
    keypad(win, true);
    WINDOW *debug = newwin(10, 15, 1, W_WIN + 5);

    FILE *read = fopen("asciiArtDB.txt", "r");
    ConvertiAsciiArt *asciiArt = new ConvertiAsciiArt(read);

    Player *player = new Player(0, 0, 20, 12, asciiArt->getFigura("PG"));
    Map *map = new Map(W_WIN - 2, H_WIN - 2);

    GestoreMondo *gestoreMondo = new GestoreMondo(6, map->getHeight(), map->getHeight(), 2, 5, asciiArt);

    ListaOggetto *listaObj = new ListaOggetto();

    for (int i = 0; i < 15; i++)
    {
        Oggetto *tmp = gestoreMondo->generaOggetto();
        aggiungiOggetto(map, listaObj, tmp);
        map->spostaVistaDestra();
    }
    for (int i = 0; i < 15; i++)
    {
        map->spostaVistaSinistra();
    }

#pragma endregion

    box(debug, 0, 0);

    int sec = 0;
    bool aggiorna = false;
    int c = -1;
    int prev = -1;
    int IDLE_TIME = 5000;
    int idle = IDLE_TIME;

    while (true)
    {

        aggiorna = false;
        c = wgetch(win);

        if (c == 10)
        {
            break;
        }

        if (c != prev)
        {
            idle = IDLE_TIME;
        }

        gestioneGravitaESalto(sec, c, &prev, &aggiorna, map, player, listaObj, gestoreMondo);

        // Gestisco gli input da tastiera
        elaboraInput(c, &prev, map, player, listaObj, asciiArt, gestoreMondo);

        // aggiornamento automatico
        if (sec % SCREEN_CLOCK == 0 || c != -1)
        {
            aggiorna = true;
            sec = 0;
        }

        if (idle == 0)
        {
            player->setFigura(asciiArt->getFigura("PG"));
            aggiorna = true;
        }

        if (aggiorna)
        {
            aggiornaSchermo(win, debug, map, player);
        }

        werase(debug);
        box(debug, 0, 0);
        mvwprintw(debug, 1, 1, "SEC : %d", sec);
        mvwprintw(debug, 2, 1, "C : %d", c);
        mvwprintw(debug, 3, 1, "P : %d", prev);
        mvwprintw(debug, 4, 1, "I : %d", idle);
        mvwprintw(debug, 5, 1, "X : %d", player->getX());
        mvwprintw(debug, 6, 1, "Y : %d", player->getY());
        mvwprintw(debug, 7, 1, "H : %d", player->getSaltaInt());
        mvwprintw(debug, 8, 1, "TERRA : %d", player->getATerra());
        wrefresh(debug);

        mvprintw(0, 40, "SIZE %d", listaObj->getSize());
        sec++;
        if (idle > 0)
            idle--;
        if (sec % INPUT_CLOCK == 0)
        {
            prev = -1;
        }
    }

    getch();
    endwin(); /* End curses mode		  */

    return 0;
}
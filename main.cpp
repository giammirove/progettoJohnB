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

const int MOV_LATERALE_IN_ARIA = 2;

void aggiungiOggetto(Map *map, ListaOggetto *listaObj, Oggetto *obj)
{
    map->aggiungiOggetto(*obj);
    listaObj->aggiungi(obj);
}

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

void disegnaPlayer(WINDOW *win, Player player)
{
    figura fig = player.getFigura();
    while (fig != NULL)
    {
        mvwprintw(win, fig->y + 1, fig->x + 1, fig->c);
        fig = fig->next;
    }
}

void elaboraInput(int c, int *prev, Map *map, Player *player, ListaOggetto *listaObj, ConvertiAsciiArt *asciiArt, GestoreMondo *gestoreMondo)
{

    if (c == KEY_UP)
    {
        if (GestoreMovimento::possoSu(map, player, listaObj))
        {
            player->salta();

            if (*prev == KEY_RIGHT)
            {
                player->resettaSaltoDestraSinistra();
                player->saltaDestra();
                player->setFigura(asciiArt->getFigura("PG_DX"));
            }
            else if (*prev == KEY_LEFT)
            {
                player->resettaSaltoDestraSinistra();
                player->saltaSinistra();
                player->setFigura(asciiArt->getFigura("PG_SX"));
            }
            else
            {
                player->setFigura(asciiArt->getFigura("PG"));
                *prev = c;
            }
        }
        /*
        if (map->dentroMargine(player->getFigura(), 0, -1))
        {
            int id_coll = map->controllaCollisione(player->getFigura(), 0, -1);
            bool solido = (id_coll != -1) ? listaObj->getDaId(id_coll).getSolido() : solido = false;
            if (id_coll == -1 || (id_coll != -1 && solido == false))
            {
                
                //player->vaiInAlto();
            }
        }
        */
    }
    if (c == KEY_LEFT)
    {

        int possoSinistra = GestoreMovimento::possoSinistra(map, player, listaObj);
        if (map->possoSpostareVistaSinistra(listaObj, player->getFigura(), player->getView()))
        {
            map->spostaVistaSinistra();
            if (*prev == KEY_UP)
            {
                player->resettaSaltoDestraSinistra();
                player->saltaSinistra();
            }
            if (map->getOffset() == 0 && possoSinistra)
            {
                player->vaiASinistra();
            }
            player->setFigura(asciiArt->getFigura("PG_SX"));
            *prev = c;
        }
        else if (possoSinistra)
        {
            if (*prev == KEY_UP)
            {
                player->resettaSaltoDestraSinistra();
                player->saltaSinistra();
            }
            else
            {
                player->vaiASinistra();
            }
            player->setFigura(asciiArt->getFigura("PG_SX"));
            *prev = c;
        }
    }
    if (c == KEY_RIGHT)
    {
        int possoDestra = GestoreMovimento::possoDestra(map, player, listaObj);
        if (map->possoSpostareVistaDestra(listaObj, player->getFigura(), player->getView()))
        {
            map->spostaVistaDestra();
            Oggetto *tmp = gestoreMondo->generaOggetto();
            aggiungiOggetto(map, listaObj, tmp);

            if (*prev == KEY_UP)
            {
                player->resettaSaltoDestraSinistra();
                player->saltaDestra();
            }
            player->setFigura(asciiArt->getFigura("PG_DX"));
            *prev = c;
        }
        else if (possoDestra)
        {
            if (*prev == KEY_UP)
            {
                player->resettaSaltoDestraSinistra();
                player->saltaDestra();
            }
            else
            {
                player->vaiADestra();
            }
            player->setFigura(asciiArt->getFigura("PG_DX"));
            *prev = c;
        }
    }
    if (c == 't')
    {
        aggiungiOggetto(map, listaObj, new Oggetto(player->getX() + player->getWidth() + map->getOffset() + 1, player->getY(), OS_PIATTAFORMA, asciiArt));
    }
    if (c == 'r')
    {
        aggiungiOggetto(map, listaObj, new Oggetto(player->getX() + player->getWidth() + map->getOffset() + 1, player->getY(), OS_WINDMILL, asciiArt));
    }
    if (c == ' ')
    {
        int id = map->controllaCollisionePiattaforme(player->getFigura(), 1, 0);
        if (id != -1)
        {
            map->rimuoviOggetto(listaObj->getDaId(id));
            listaObj->rimuoviDaId(id);
        }
    }
    if (c == 'k')
    {
        Oggetto *tmp = gestoreMondo->generaOggetto();
        aggiungiOggetto(map, listaObj, tmp);
    }
    if (c == 'p')
    {
        player->setFigura(asciiArt->getFigura("RINO"));
    }
    if (c == 'o')
    {
        player->setFigura(asciiArt->getFigura("BAT"));
    }
    if (c == 'i')
    {
        player->setFigura(asciiArt->getFigura("FROG"));
    }

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

void aggiornaSchermo(WINDOW *win, WINDOW *debug, Map *map, Player *player)
{
    //erase();
    werase(win);
    werase(debug);

    box(win, 0, 0);
    box(debug, 0, 0);
    disegnaMappa(win, map);

    disegnaPlayer(win, *player);

    refresh();
    wrefresh(win);
    wrefresh(debug);
}

void gestioneGravitaESalto(int sec, int c, int *prev, bool *aggiorna, Map *map, Player *player, ListaOggetto *listaObj)
{

    // Qui sta continuando il salto
    if (player->getSaltaInt() > (player->getSaltaHeight() - MOV_LATERALE_IN_ARIA) / 2 + MOV_LATERALE_IN_ARIA)
    {
        if (sec % player->getClock() == 0)
        {
            if (GestoreMovimento::possoSu(map, player, listaObj))
            {
                if (player->getSaltaDestra())
                {
                    if (GestoreMovimento::possoDestra(map, player, listaObj))
                    {
                        if (map->possoSpostareVistaDestra(listaObj, player->getFigura(), player->getView()))
                        {
                            map->spostaVistaDestra();
                        }
                        else
                        {
                            player->vaiADestra();
                        }
                    }
                    else
                    {
                        player->resettaSalto();
                        //*prev = -1;
                    }
                }
                if (player->getSaltaSinistra())
                {
                    if (GestoreMovimento::possoSinistra(map, player, listaObj))
                    {
                        if (map->possoSpostareVistaSinistra(listaObj, player->getFigura(), player->getView()))
                        {
                            map->spostaVistaSinistra();
                        }
                        else
                        {
                            player->vaiASinistra();
                        }
                    }
                    else
                    {
                        player->resettaSalto();
                        //*prev = -1;
                    }
                }

                player->vaiInAlto();
                player->decrementaSalto();

                *aggiorna = true;
            }
            else
            {
                player->resettaSalto();
                //*prev = -1;
            }
        }
    }
    // quando arriva in alto si sposta di uno a destra o sinistra
    else if (player->getSaltaInt() >= ((player->getSaltaHeight() - MOV_LATERALE_IN_ARIA) / 2) && player->getSaltaInt() <= ((player->getSaltaHeight() - MOV_LATERALE_IN_ARIA) / 2 + MOV_LATERALE_IN_ARIA))
    {
        if (sec % player->getClock() == 0)
        {
            if (player->getSaltaDestra())
            {
                if (GestoreMovimento::possoDestra(map, player, listaObj))
                {
                    if (map->possoSpostareVistaDestra(listaObj, player->getFigura(), player->getView()))
                    {
                        map->spostaVistaDestra();
                    }
                    else
                    {
                        player->vaiADestra();
                    }
                }
                else
                {
                    player->resettaSalto();
                    //*prev = -1;
                }
            }
            if (player->getSaltaSinistra())
            {
                if (GestoreMovimento::possoSinistra(map, player, listaObj))
                {
                    if (map->possoSpostareVistaSinistra(listaObj, player->getFigura(), player->getView()))
                    {
                        map->spostaVistaSinistra();
                    }
                    else
                    {
                        player->vaiASinistra();
                    }
                }
                else
                {
                    player->resettaSalto();
                    //*prev = -1;
                }
            }

            player->decrementaSalto();
            *aggiorna = true;
        }
    }
    // Controllo se posso scendere
    else if (sec % 400 == 0)
    {
        if (GestoreMovimento::possoGiu(map, player, listaObj))
        {
            player->vaiInBasso();
            // controllo se sto ancora scendendo da dx o sxs
            if (player->getSaltaInt() > 0)
            {
                if (player->getSaltaDestra())
                {
                    if (GestoreMovimento::possoDestra(map, player, listaObj))
                    {
                        if (map->possoSpostareVistaDestra(listaObj, player->getFigura(), player->getView()))
                        {
                            map->spostaVistaDestra();
                        }
                        else
                        {
                            player->vaiADestra();
                        }
                        *prev = KEY_RIGHT;
                    }
                    else
                    {
                        player->resettaSalto();
                        //*prev = -1;
                    }
                }
                if (player->getSaltaSinistra())
                {
                    if (GestoreMovimento::possoSinistra(map, player, listaObj))
                    {
                        if (map->possoSpostareVistaSinistra(listaObj, player->getFigura(), player->getView()))
                        {
                            map->spostaVistaSinistra();
                        }
                        else
                        {
                            player->vaiASinistra();
                        }
                        *prev = KEY_LEFT;
                    }
                    else
                    {
                        player->resettaSalto();
                        //*prev = -1;
                    }
                }
                player->decrementaSalto();
            }
            else
            {
                //*prev = -1;
                player->resettaSaltoDestraSinistra();
            }

            *aggiorna = true;
        }
        else
        {
            player->resettaSalto();
            *prev = -1;
        }
    }
}

int main()
{
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

    int h_win = 20;
    int w_win = 60;
    WINDOW *win = newwin(h_win, w_win, 1, 1);
    nodelay(win, true);
    scrollok(win, TRUE);
    keypad(win, true);
    WINDOW *debug = newwin(10, 15, 1, w_win + 5);

    FILE *read = fopen("asciiArtDB.txt", "r");
    ConvertiAsciiArt *asciiArt = new ConvertiAsciiArt(read);

    Player *player = new Player(0, 0, 20, 15, asciiArt->getFigura("PG"));
    Map *map = new Map(w_win - 2, h_win - 2);

    GestoreMondo *gestoreMondo = new GestoreMondo(6, map->getHeight(), 10, 2, 5, asciiArt);

    ListaOggetto *listaObj = new ListaOggetto();

    for (int i = 0; i < 8; i++)
    {
        Oggetto *tmp = gestoreMondo->generaOggetto();
        aggiungiOggetto(map, listaObj, tmp);
    }

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

        gestioneGravitaESalto(sec, c, &prev, &aggiorna, map, player, listaObj);

        // Gestisco gli input da tastiera
        elaboraInput(c, &prev, map, player, listaObj, asciiArt, gestoreMondo);

        // aggiornamento automatico
        if (sec % SCREEN_CLOCK == 0 || c != -1)
        {
            aggiorna = true;
            sec = 0;
        }

        if (idle == 0) {
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
        mvwprintw(debug, 8, 1, player->getFigura()->next->next->c);
        wrefresh(debug);

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
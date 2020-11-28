#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "map.h"
#include "player.h"
#include "oggetto.h"
#include "listaOggetto.h"

const int SCREEN_CLOCK = 100000000;

void salto(WINDOW *win, WINDOW *debug)
{

    for (int i = 5; i >= 0; i--)
    {
        wclear(debug);
        box(debug, 0, 0);
        mvwprintw(debug, (i), 2, "\U0001F333");
        refresh();
        wrefresh(win);
        wrefresh(debug);
        usleep(75000);
    }
    usleep(10000);
    for (int i = 0; i < 5; i++)
    {
        wclear(debug);
        box(debug, 0, 0);
        mvwprintw(debug, (i), 2, "\U0001F333");
        refresh();
        wrefresh(win);
        wrefresh(debug);
        usleep(60000);
    }
}

void disegnaMappa(WINDOW *win, Map *map)
{
    mappa_t m = map->getMappa();
    printw("offset %d", map->getOffset());
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

void disegnaPlayer(WINDOW *win, Player player) {
    figura fig = player.getFigura();
    while(fig != NULL) {
        mvwprintw(win, fig->y + 1, fig->x + 1, fig->c);
        fig = fig->next;
    }
}

void elaboraInput(int c, Map *map, Player *player, ListaOggetto *listaObj)
{

    if (c == KEY_UP)
    {
        if (map->dentroMargine(player->getX(), player->getY() - 1))
        {
            int id_coll = map->controllaCollisione(player->getFigura(), 0, -1);
            bool solido = (id_coll != -1) ? listaObj->getDaId(id_coll).getSolido() : solido = false;
            if (id_coll == -1 || (id_coll != -1 && solido == false))
            {
                player->vaiInAlto();
            }
        }
    }
    if (c == KEY_DOWN)
    {
        if (map->dentroMargine(player->getX(), player->getY() + 1))
        {
            int id_coll = map->controllaCollisione(player->getFigura(), 0, +1);
            bool solido = (id_coll != -1) ? listaObj->getDaId(id_coll).getSolido() : solido = false;
            if (id_coll == -1 || (id_coll != -1 && solido == false))
                player->vaiInBasso();
        }
    }
    if (c == KEY_LEFT)
    {
        int id_coll = map->controllaCollisione(player->getFigura(), -1, 0);
        // affinche la visuale si sposti il player non deve collidere
        if (id_coll == -1 && !map->dentroMargine(player->getX() - player->getView(), player->getY()) && map->getOffset() != 0)
            map->spostaVistaSinistra();

        else if (map->dentroMargine(player->getX() - 1, player->getY()))
        {
            bool solido = (id_coll != -1) ? listaObj->getDaId(id_coll).getSolido() : solido = false;
            if (id_coll == -1 || (id_coll != -1 && solido == false))
                player->vaiASinistra();
        }
    }
    if (c == KEY_RIGHT)
    {
        int id_coll = map->controllaCollisione(player->getFigura(), 1, 0);
        // affinche la visuale si sposti il player non deve collidere
        if (id_coll == -1 && !map->dentroMargine(player->getX() + player->getView(), player->getY()))
            map->spostaVistaDestra();

        else if (map->dentroMargine(player->getX() + 1, player->getY()))
        {
            bool solido = (id_coll != -1) ? listaObj->getDaId(id_coll).getSolido() : solido = false;
            if (id_coll == -1 || (id_coll != -1 && solido == false))
                player->vaiADestra();
        }
    }
    if (c == 't')
    {
        Oggetto *tmpObj = new Oggetto(player->getX() + map->getOffset() + 1, player->getY(), OS_ALBERO);
        map->aggiungiOggetto(*tmpObj);
        listaObj->aggiungi(tmpObj);
    }
    if (c == 'r')
    {
        Oggetto *tmpObj = new Oggetto(player->getX() + map->getOffset() + 1, player->getY(), OS_CESPUGLIO);
        map->aggiungiOggetto(*tmpObj);
        listaObj->aggiungi(tmpObj);
    }
    if (c == ' ')
    {
        int id = map->controllaCollisione(player->getFigura(), 1, 0);
        if (id != -1)
        {
            map->rimuoviOggetto(listaObj->getDaId(id));
            listaObj->rimuoviDaId(id);
        }
    }

    // CONTROLLA COLLISIONE
    if (map->controllaCollisione(player->getFigura()) != -1)
    {
        int id = map->controllaCollisione(player->getFigura());
        int tipo = listaObj->getDaId(id).getTipoDiOggetto();
        printw("\tCOLLISIONE : %d (x:%d) (y:%d)", id, player->getX(), player->getY());
        if (tipo == 0)
            printw(" TIPO : ALBERO");
        else if (tipo == 1)
            printw(" TIPO : CESPUGLIO");
    }
}

void aggiornaSchermo(WINDOW *win, WINDOW *debug, Map *map, Player *player)
{
    erase();
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

int main()
{
    setlocale(LC_ALL, "");
    //const wchar_t *wstr = L"\u2603\u26c4\U0001F638";
    initscr();
    timeout(1);
    //initscr(); /* Start curses mode 		  */
    noecho();
    cbreak();

    // nasconde il cursore
    curs_set(0);

    int h_win = 15;
    int w_win = 40;
    WINDOW *win = newwin(h_win, w_win, 1, 1);
    nodelay(win, true);
    WINDOW *debug = newwin(10, 10, 1, w_win + 5);

    Player *player = new Player(0, 0, 3);
    Map *map = new Map(w_win - 2, h_win - 2);

    ListaOggetto *listaObj = new ListaOggetto();
    Oggetto *alb2 = new Oggetto(7, 5, OS_CESPUGLIO);
    Oggetto *alb = new Oggetto(10, 3, OS_ALBERO);
    Oggetto *alb3 = new Oggetto(0, 3, OS_KIRBY);
    Oggetto *alb4 = new Oggetto(2, 6, OS_PIATTAFORMA);

    map->aggiungiOggetto(*alb2);
    map->aggiungiOggetto(*alb);
    map->aggiungiOggetto(*alb3);
    map->aggiungiOggetto(*alb4);
    listaObj->aggiungi(alb);
    listaObj->aggiungi(alb2);
    listaObj->aggiungi(alb3);
    listaObj->aggiungi(alb4);

    printw("\U0001F333");
    box(debug, 0, 0);

    keypad(win, true);

    int sec = 0;
    while (true)
    {
        int c = wgetch(win);

        if (c == 10)
        {
            break;
        }
        if (false && c == -1 && sec % player->getClock() == 0 && player->getY() < map->getHeight() - 1 && map->controllaCollisione(player->getX(), player->getY() + 1) == -1)
        {
            player->vaiInBasso();
            aggiornaSchermo(win, debug, map, player);
        }
        elaboraInput(c, map, player, listaObj);
        if (sec % SCREEN_CLOCK == 0 || c != -1)
        {
            aggiornaSchermo(win, debug, map, player);
        }

        werase(debug);
        wprintw(debug, "SEC : %d \nC : %d", sec, c);
        wrefresh(debug);

        c = 0;
        sec++;
        if (sec > SCREEN_CLOCK * 10)
            sec = 0;
    }

    getch();
    endwin(); /* End curses mode		  */

    return 0;
}
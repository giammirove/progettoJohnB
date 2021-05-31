#include <ncurses.h>
#include <curses.h>
#include <locale.h>
#include <string.h>
#include <unistd.h>
#include <typeinfo>
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
#include "listaNemici.h"
#include "nemico.h"
#include "gioco.h"

using namespace std;

int H_WIN = 27;
int W_WIN = 60;

/*
    Disegna i punti vita del player e score
*/
void disegnaScoreEVita(WINDOW *win, Player *player)
{
    for (int i = 0; i < player->getVita() * 2; i += 2)
    {
        mvwprintw(win, 1, i + 1, "♥");
    }
    mvwprintw(win, 2, 1, "Score %d", player->getScore());
    if (player->getArmaAttiva())
    {
        mvwprintw(win, 1, W_WIN - 12, "ARMA %3d", player->getValoreArma());
    }
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
            // NEMICO
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
    if (player.getInvulnerabile() && player.getValoreInvulnerabile() % 2 == 0)
        wattron(win, COLOR_PAIR(1));
    while (fig != NULL)
    {
        mvwprintw(win, fig->y + 1, fig->x + 1, fig->c);
        fig = fig->next;
    }
    if (player.getInvulnerabile() && player.getValoreInvulnerabile() % 2 == 0)
        wattroff(win, COLOR_PAIR(1));
    if (player.getArmaAttiva())
    {
        figura arma = player.getArma()->getFigura();
        while (arma != NULL)
        {
            mvwprintw(win, arma->y + 1, arma->x + 1, arma->c);
            arma = arma->next;
        }
    }
}

/*
    Disegna la lava 
*/
void disegnaLava(WINDOW *win)
{
    for (int i = 1; i < W_WIN-1; i++)
    {
        mvwaddch(win, H_WIN - 2, i, '~' | COLOR_PAIR(1));
    }
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
    wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
    //box(debug, 0, 0);
    // Disegno la mappa nella finestra win
    disegnaMappa(win, map);

    // Disegno il player nella finestra win
    disegnaPlayer(win, *player);

    // Disegno score e vita del player
    disegnaScoreEVita(win, player);

    // Disegno la lava
    disegnaLava(win);

    // Aggiorno per apportare le modifiche
    refresh();
    wrefresh(win);
    wrefresh(debug);
}

/*
    Schermata di perdita
*/
void schermataDiPerdita(WINDOW *win, Gioco *gioco)
{
    // aspetta qualche istante
    usleep(150000);

    // Elimino ciò che era presente prima
    werase(win);
    // Ridisegno i bordi
    wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);

    // Disegno il teschio
    figura teschio = gioco->getAsciiArt()->getFigura("DEATH");
    while (teschio != NULL)
    {
        mvwprintw(win, teschio->y+1, teschio->x+8, teschio->c);
        teschio = teschio->next;
        //mvwprintw(win, (H_WIN / 2), (W_WIN / 2) - 5, "HAI PERSO!");
    }

    // Aggiorno per apportare le modifiche
    wrefresh(win);
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
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);

#pragma endregion

#pragma region INIT

    WINDOW *win = newwin(H_WIN, W_WIN, 1, 1);
    nodelay(win, true);
    scrollok(win, TRUE);
    keypad(win, true);
    WINDOW *debug = newwin(15, 15, 1, W_WIN + 5);

    FILE *read = fopen("asciiArtDB.txt", "r");
    Gioco *gioco = new Gioco(read, H_WIN, W_WIN);

#pragma endregion

    box(debug, 0, 0);

    bool aggiorna = false;
    int c = -1;
    int prev = -1;
    int idle = gioco->getIdleTime();
    int start = clock();
    int current_clock = start;

    while (true && gioco->getPlayer()->getVita() > 0)
    {
        // 1 / 10 di secondo
        current_clock = ((clock() - start)/(CLOCKS_PER_SEC/1000));

        if (gioco->getPlayer()->toccoLaLava(gioco->getMap()->getHeight()))
        {
            gioco->getPlayer()->muori();
            continue;
        }

        aggiorna = false;
        c = wgetch(win);

        if (c == 10)
        {
            //break;
        }

        if (c != prev)
        {
            idle = gioco->getIdleTime();
        }

        gioco->gestisciGioco(c, &prev, current_clock, &aggiorna);

        if (c != -1)
        {
            aggiorna = true;
        }

        if (idle == 0)
        {
            gioco->getPlayer()->setFigura(gioco->getAsciiArt()->getFigura("PG"));
            aggiorna = true;
        }

        if (aggiorna)
        {
            aggiornaSchermo(win, debug, gioco->getMap(), gioco->getPlayer());
        }   

        //mvprintw(0,0, "TIME : %3d - %3d", current_clock, sec);

        /*
        werase(debug);
        box(debug, 0, 0);
        mvwprintw(debug, 1, 1, "SEC : %d", sec);
        mvwprintw(debug, 2, 1, "C : %d", c);
        mvwprintw(debug, 3, 1, "P : %d", prev);
        mvwprintw(debug, 4, 1, "I : %d", idle);
        mvwprintw(debug, 5, 1, "X : %d", gioco->getPlayer()->getX() + gioco->getMap()->getOffset());
        mvwprintw(debug, 6, 1, "Y : %d", gioco->getPlayer()->getY());
        mvwprintw(debug, 7, 1, "H : %d", gioco->getPlayer()->getSaltaInt());
        mvwprintw(debug, 8, 1, "TERRA : %d", gioco->getPlayer()->getATerra());
        mvwprintw(debug, 9, 1, "SCENDO : %d", gioco->getPlayer()->stoScendendo());
        mvwprintw(debug, 10, 1, "OFFSET : %d", gioco->getMap()->getOffset());
        mvwprintw(debug, 11, 1, "DIR : %d", gioco->getPlayer()->getArma()->getDirezione());
        mvwprintw(debug, 12, 1, "SIZE : %d", gioco->getListaObj()->getSize());
        mvwprintw(debug, 13, 1, "INV : %4d", gioco->getPlayer()->getInvulnerabile());
        mvwprintw(debug, 14, 1, "NEM : %d", nemClock);
        wrefresh(debug);
        */

        if (idle > 0)
            idle--;
        if (current_clock % gioco->getInputClock() == 0)
        {
            prev = -1;
        }
        if(current_clock >= gioco->getMaxClock()) {
            start = clock();
            current_clock = start;
        }
    }

    schermataDiPerdita(win, gioco);

    while (true)
        ;

    getch();
    endwin(); /* End curses mode		  */

    return 0;
}
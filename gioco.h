#ifndef GIOCO
#define GIOCO

#include <ncurses.h>
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
#include "listaBonus.h"
#include "bonus.h"

class Gioco
{
private:

    #pragma region  Oggetti
    Player *player;
    ConvertiAsciiArt *asciiArt;
    GestoreMondo *gestoreMondo;
    Map *map;
    ListaOggetto *listaObj;
    ListaNemici *listaNem;
    ListaBonus *listaBonus;
    #pragma endregion

    #pragma region Constanti

    const int SCREEN_CLOCK = 100000;
    const int INPUT_CLOCK = 20000;
    const int NEMICI_CLOCK = 1000;
    const int MAX_CLOCK_NEMICI = 2000;

    const int IDLE_TIME = 5000;
    const int MAX_SEC = 2000;

    const int NUM_PIATTAFORME = 10;
    const int NUM_NEMICI = 10;

    int H_WIN = 24;
    int W_WIN = 60;

    const int MOV_LATERALE_IN_ARIA = 2;

    #pragma endregion

public:
    Gioco(FILE *read);

    Player *getPlayer();
    ConvertiAsciiArt *getAsciiArt();
    GestoreMondo *getGestoreMondo();
    Map *getMap();
    ListaOggetto *getListaObj();
    ListaNemici *getListaNem();
    ListaBonus *getListaBonus();

    void gestisciGioco(int c, int *prev, int sec, int nemClock, bool*aggiorna);

    void elaboraInput(int c, int *prev);

    void gestioneCollisioneNemiciEArmi();
    void gestioneGravitaESalto(int sec, int c, int *prev, bool *aggiorna);
    void gestioneNemici(int sec, bool *aggiorna);

    void aggiungiBloccoAlMondo();
    void aggiungiNemico();
    void aggiungiNemico(Nemico *nem);
    void rimuoviNemicoDaId(int id);
    void aggiungiOggetto(Oggetto *obj);
    void rimuoviOggettoDaId(int id);
    void aggiungiBonus(Nemico *nem);
    void aggiungiBonus(Bonus *bonus);
    void rimuoviBonusDaId(int id);

    void applicaBonus(Bonus *bonus);
};

#endif
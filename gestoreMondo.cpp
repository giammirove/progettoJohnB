#include <string.h>
#include <locale.h>
#include <ncurses.h>
#include <cstdlib>
#include "gestoreMondo.h"
#include "oggetto.h"
#include "utility.h"
#include "nemico.h"

/*
    Costruttore della classe GestoreMondo
    asciiArt = classe che permette di ottenere le figure in base al nome
    Costruttore di default che inizializza tutto a valori prestabiliti
*/
GestoreMondo::GestoreMondo(ConvertiAsciiArt *asciiArt) {
    _width = 5; 
    _map_h = 18;
    _max_h = 10;
    _min_h = 2;
    _saltoPlayer = 5;
    _numeroPiattaforme = 2;
    _lastXPavimento = 0;
}

/*
    Costruttore della classe GestoreMondo
    _w = indica la _width ovvero la dimensione di ogni chunk
    map_h = indica la _map_h ovvero l'altezza della mappa
    max_h = indica la _max_h ovvero l'altezza massima in cui puo comparire la piattaforma (forse non serve, da rimuovere)
    min_h = indica la _min_h ovvero l'altezza minima dalla quale le piattaforme possono comparire (utile per pavimento)
    saltoPlayer = _saltoPlayer ovvero quanto salta il player (il calcolo approssimativo è dato da Player.getSaltaHeight() / 2 + 1)
    asciiArt = classe che permette di ottenere la figure in base al nome
*/
GestoreMondo::GestoreMondo (int _w, int map_h, int max_h, int min_h, int saltoPlayer, int numeroPiattaforme, int numeroNemici, ConvertiAsciiArt *asciiArt) {
    _width = _w;
    _map_h = map_h;
    _max_h = max_h;
    _min_h = min_h;
    _saltoPlayer = saltoPlayer;
    _asciiArt = asciiArt;
    _numeroPiattaforme = numeroPiattaforme;
    _numeroNemici = numeroNemici;
    _lastXPavimento = 0;
}

/*
    Genera un nuovo oggetto e lo posiziona casualmente in base all'oggetto 
    generato precedentemente
*/
Oggetto *GestoreMondo::generaOggetto () {

    _lastXPavimento += _width;

    // rnd indica il tipo di oggetto casuale da generare
    // i primi n oggetti dell'enum saranno usato per 
    // questa funzione
    // in questa caso n è uguale 2
    int rnd = rand() % 2;
    int start_x = 0;
    if(_oggettoPrec != NULL)
        start_x = _oggettoPrec->getFigura()->x;
    else 
        start_x = 4;
    
    // La x generata casualmente si trova nell'intervallo [start_x, start_x + _width]
    // in questo modo sarà sicuramente dopo la struttura precedente ma all'interno del chunk
    int rnd_x = start_x + rand () % _width;
    // abbiamo dato per scontato il salto del player come costante
    int rnd_y = -1;
    // verifica che la y abbastanza grande da permettere al player di saltare 
    // e sia abbastanza piccola da non andare oltre le dimensioni della mappa 
    while(rnd_y < _saltoPlayer || rnd_y > _map_h - _min_h) {
        if(_oggettoPrec != NULL)
            // la nuova y si trova l'intervallo [y precedente - saltoPlayer, (y precedente - saltoPlayer) + 2 * saltoPlayer]
            rnd_y = randomNumber((_oggettoPrec->getFigura()-> y - _saltoPlayer), (_oggettoPrec->getFigura()-> y + _saltoPlayer));
        else 
            rnd_y = _map_h - _min_h;
    }
    // ci saranno dei controlli qua per rnd_x e rnd_y
    Oggetto *tmp = new Oggetto(rnd_x, rnd_y, (TipoDiOggetto)rnd, _asciiArt);
    _oggettoPrec = tmp;

    mvprintw(0,0, "%d - (%d < %d < %d)", rnd_x, (_saltoPlayer), rnd_y, _map_h - _min_h);

    return tmp;
}

/*
    Genera un numerico sulla piattaforma precedente
*/
Nemico *GestoreMondo::generaNemico(){

    int rnd = randomNumber(_numeroPiattaforme, _numeroPiattaforme+1);
    Nemico *tmp = new Nemico(_oggettoPrec->getFigura()->x, _oggettoPrec->getFigura()->y, (TipoDiOggetto)rnd, _asciiArt);
    int rnd_x = randomNumber(-(_oggettoPrec->getWidth()), -(tmp->getWidth()));
    tmp->muoviFigura(rnd_x, -(tmp->getHeight()+1));

    //mvprintw(0, 100, "(%6d - %6d)", _oggettoPrec->getWidth(), tmp->getWidth());
    return tmp;
}

/*
    Chiede se deve generare il pavimento
*/
bool GestoreMondo::generoPavimento(){
    return randomNumber(0, 10) != 10;
}

/*
    Ottiene la X alla quale far comparire il pavimento
*/
int GestoreMondo::getXPavimento(){
    return _lastXPavimento;
}
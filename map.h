#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include <string.h>
#include "listaOggetto.h"
#include "oggetto.h"

/*
    FUZIONAMENTO
    Es. mappa 5x5
    0   1   2   3   4
    []->[]->[]->[]->[]
    ↓   ↓   ↓   ↓   ↓
  0 []  []  []  []  []
    ↓   ↓   ↓   ↓   ↓
  1 []  []  []  []  []
    ↓   ↓   ↓   ↓   ↓
  2 []  []  []  []  []
    ↓   ↓   ↓   ↓   ↓
  3 []  []  []  []  []
    ↓   ↓   ↓   ↓   ↓
  4 []  []  []  []  []

    In pratica si tratta di una lista di liste
    La mappa è formata da una lista di colonne, 
    in questo modo è più facile aggiungerle di vola in volta
    Una colonna è formata da una lista di righe
    Con questo sistema inoltre non c'è bisogno di scorrere
    tutta la lista di liste controllando ogni singola coordinata,
    ma solo la lista con l'ordinata corretta
*/

struct riga_t
{
    int y;
    char c[30];
    int id;
    riga_t *next;
};

typedef struct riga_t *riga;

struct colonna_t
{
    int x;
    riga r;
    colonna_t *next;
};

typedef struct colonna_t *mappa_t;

class Map
{
public:
    Map(int width, int height);
    //~Map();

    void initMap();
    mappa_t getMappa();
    int getWidth();
    int getHeight();
    int getView();
    int getOffset();
    void setPunto(const char c[], int x, int y);
    void setPunto(const char c[], int x, int y, int id);
    char *getPunto(int x, int y);
    int controllaCollisione(int x, int y);
    int controllaCollisione(figura fig);
    int controllaCollisione(figura fig, int inc_x, int inc_y);
    int controllaCollisionePiattaforme(figura fig);
    int controllaCollisionePiattaforme(figura fig, int inc_x, int inc_y);
    bool dentroMargine(int x, int y);
    bool dentroMargine(figura fig);
    bool dentroMargine(figura fig, int inc_x, int inc_y);

    void spostaVistaDestra();
    bool possoSpostareVistaDestra(ListaOggetto *listaObj, figura fig, int view);
    void spostaVistaSinistra();
    bool possoSpostareVistaSinistra(ListaOggetto *listaObj, figura fig, int view);

    void aggiungiOggetto(Oggetto obj);
    void rimuoviOggetto(Oggetto obj);

private:
    int _view; // indica la dimensione della vista
    int _offset; // indica di quanto la vista si è spostata
    int _init_width;
    int _width;
    int _init_height;
    mappa_t map;

    void aggiungiRiga(riga *r, int y);

    void creaColonna(int x);

    void aggiungiColonna(int x);
};

#endif
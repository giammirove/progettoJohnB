#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include <string.h>

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
    char c[2];
    riga_t *next;
};

typedef struct riga_t *riga;

struct colonna_t
{
    int x;
    riga r;
    colonna_t *next;
};

typedef struct colonna_t *mappa;

class Map
{
public:
    Map(int width, int height);

    void initMap();
    mappa getMappa();
    void setPunto(const char c[], int x, int y);
    char *getPunto(int x, int y);

private:
    int _width;
    int _height;
    mappa map;

    void aggiungiRiga(riga *r, int y);

    void aggiungiColonna(mappa *map, int x);
};

#endif
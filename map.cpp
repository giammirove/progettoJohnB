#include <fstream>
#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include "map.h"
#include "oggetto.h"

using namespace std;

// PUBLIC

/*
    Costruttor della classe Map
    width = indica la lunghezza iniziale della mappa
    height = indica la altezza iniziale della mappa
    offset viene impostato a 0
*/
Map::Map(int width, int height)
{
    // view si inizializza come la prima dimensione della mappa
    _view = width;
    _offset = 0;
    _width = width;
    _init_width = width;
    _init_height = height;

    initMap();
}

/*
    Inizializza la mappa creando tante colonne quanto è la lunghezza iniziale
*/
void Map::initMap()
{
    map = new colonna_t;
    for (int i = 0; i < _init_width; i++)
    {
        aggiungiColonna(i);
    }
}

/*
    GetMappa ritorna la mappa che attualmente il player sta guardando
    ovvero scelta in base all'offset
*/
mappa_t Map::getMappa()
{
    return view_map;
}

/*
    Calcola la mappa che attualmente il player sta guardando
*/
mappa_t Map::calcMappa()
{
    mappa_t m = map;
    int c = 0;

    // mi sposto indietro se necessario
    // offset può essere solo positivo
    int param = _width - _view - _offset;
    while (m != NULL && c < param)
    {
        m = m->next;
        c++;
    }

    if (m != NULL)
    {
        return m;
    }
    else
    {
        initMap();
        return getMappa();
    }

    return map;
}

/*
    Ottiene la mappa completa
*/
mappa_t Map::getMappaCompleta()
{
    return map;
}
/*
    ritorna la lunghezza della mappa
*/
int Map::getWidth()
{
    return this->_width;
}

/*
    ritorna l'altezza del mondo
*/
int Map::getHeight()
{
    return this->_init_height;
}

/*
    ritorna la dimensione della vista
*/
int Map::getView()
{
    return this->_view;
}

/*
    ritorna l'offset 
*/
int Map::getOffset()
{
    return this->_offset;
}

/*
    imposta un punto della mappa in una determinata coordinata x, y
*/
void Map::setPunto(const char c[], int x, int y)
{
    mappa_t m = this->map;
    bool found = false;
    // scorre tutte le colonne finche non trova la x corretta
    while (m != NULL && found == false)
    {
        if (m->x == x)
            found = true;
        else
            m = m->next;
    }

    if (m != NULL)
    {
        found = false;
        riga r = m->r;
        // scorre tutte le righe della colonna scelta finche non trova la y corretta
        while (r != NULL && found == false)
        {
            if (r->y == y)
                found = true;
            else
                r = r->next;
        }

        if (r != NULL)
            strcpy(r->c, c);
    }
}

/*
    Imposta un punto nella mappa in base alle coordinate x e y, ci associa un particolare id
    guardare commenti della funzione setPunto
*/
void Map::setPunto(const char c[], int x, int y, int id)
{
    mappa_t m = this->map;
    bool found = false;
    while (m != NULL && found == false)
    {
        if (m->x == x)
            found = true;
        else
            m = m->next;
    }

    if (m != NULL)
    {
        found = false;
        riga r = m->r;
        while (r != NULL && found == false)
        {
            if (r->y == y)
                found = true;
            else
                r = r->next;
        }

        if (r != NULL)
        {
            strcpy(r->c, c);
            r->id = id;
        }
    }
}

/*
    Ottiene il punto alle coordinate x e y
*/
char *Map::getPunto(int x, int y)
{
    mappa_t m = this->map;
    bool found = false;
    char *c_null = new char;
    strcpy(c_null, "");
    while (m != NULL && found == false)
    {
        if (m->x == x)
            found = true;
        else
            m = m->next;
    }

    if (m != NULL)
    {
        found = false;
        riga r = m->r;
        while (r != NULL && found == false)
        {
            if (r->y == y)
                found = true;
            else
                r = r->next;
        }

        if (r != NULL)
            return (r->c);
        else
            return c_null;
    }
    else
    {
        return c_null;
    }
}

/*
    Controlla se alle coordinata x e y è presente un punto
*/
int Map::controllaCollisione(int x, int y)
{
    mappa_t m = getMappa();
    bool found = false;
    int c = 0; // controlla le collisioni solo in mappa visibile altrimenti non ha senso
    while (m != NULL && found == false && c < (_view))
    {
        if (m->x == x + _offset)
            found = true;
        else
            m = m->next;
        c++;
    }

    if (m != NULL)
    {
        found = false;
        riga r = m->r;
        while (r != NULL && found == false)
        {
            if (r->y == y)
                found = true;
            else
                r = r->next;
        }

        if (r != NULL)
            return (r->id);
        else
            return -1;
    }
    else
    {
        return -1;
    }
}

/*
    Controlla se almeno un punto della figura ha una collisione, ovvero
    se almeno un punto della figura si trova in una posizione già occupata
    in mappa
*/
int Map::controllaCollisione(figura fig)
{
    return controllaCollisione(fig, 0, 0);
}

/*
    Controlla se almeno un punto della figura ha una collisione, ovvero
    se almeno un punto della figura si trova in una posizione già occupata
    in mappa
    inc_x = indica di quanto incrementare il valore x di ogni punto della figura,
            utile per verificare se la prossima posizione dopo un movimento è disponibile
    inc_y = indica di quanto incrementare il valore y di ogni punto della figura,
            utile per verificare se la prossima posizione dopo un movimento è disponibile
*/
int Map::controllaCollisione(figura fig, int inc_x, int inc_y)
{
    bool found = false;
    int res = -1;
    while (fig != NULL && found == false)
    {
        int col = controllaCollisione(fig->x + inc_x, fig->y + inc_y);
        if (col != -1)
        {
            found = true;
            res = col;
        }
        else
        {
            fig = fig->next;
        }
    }

    return res;
}

/*
    Controlla se posso effettuare un movimento su una piattaforma
    in realtà non cambia niente mi sa, da rimuovere
*/
int Map::controllaCollisionePiattaforme(figura fig)
{
    return controllaCollisionePiattaforme(fig, 0, 0);
}

int Map::controllaCollisionePiattaforme(figura fig, int inc_x, int inc_y)
{
    if (fig == NULL)
    {
        return -1;
    }
    figura _tmp = fig;
    int max = _tmp->y;
    int res = -1;
    bool found = false;
    while (_tmp != NULL)
    {
        if (_tmp->y > max)
        {
            max = _tmp->y;
        }
        _tmp = _tmp->next;
    }

    while (fig != NULL && found == false)
    {
        if (fig->y == max)
        {
            int col = controllaCollisione(fig->x + inc_x, fig->y + inc_y);
            if (col != -1)
            {
                found = true;
                res = col;
            }
            else
            {
                fig = fig->next;
            }
        }
        else
        {
            fig = fig->next;
        }
    }

    return res;
}

/*
    Aggiunge un oggetto alla mappa, ovvero aggiunge ogni punto della figura
    dell'oggetto alla mappa con relativo id
*/
void Map::aggiungiOggetto(Oggetto *obj)
{
    if (obj != NULL)
    {
        figura fig = obj->getFigura();
        while (fig != NULL)
        {
            if (strlen(fig->c) > 0)
                setPunto(fig->c, fig->x, fig->y, obj->getId());
            fig = fig->next;
        }
    }
}


/*
    Rimuove un oggetto dalla mappa, ovvero resetta ogni punto della figura
    dell'oggetto nella mappa
*/
void Map::rimuoviOggetto(Oggetto *obj)
{
    if (obj != NULL)
    {
        figura fig = obj->getFigura();
        while (fig != NULL)
        {
            if (strlen(fig->c) > 0)
                setPunto("", fig->x, fig->y, -1);
            fig = fig->next;
        }
    }
}


/*
    sposta la vista destra, quindi incrementa l'offset di 1
    e precarica 10 colonne
*/
void Map::spostaVistaDestra()
{
    this->_offset++;
    // aggiungo 5 colonne ovvero _width di gestore mondo
    for (int i = 0; i < 30; i++)
    {
        aggiungiColonna(_width);
        _width++;
    }
    view_map = calcMappa();
}

/*
    Verifica se è possibile spostare la vista a destra, ovvero 
    se il player non è in collisione con nulla alla sua destra
    view = indica l'area del player
    view view
    ----@----
*/
bool Map::possoSpostareVistaDestra(ListaOggetto *listaObj, figura fig, int view)
{
    int id_coll = controllaCollisione(fig, 1, 0);
    bool solid = true;
    if (id_coll != -1)
        solid = listaObj->getDaId(id_coll)->getSolido();
    return ((id_coll == -1 || solid == false) && !dentroMargine(fig, view, 0));
}

/*
    Sposta la vista a sinistra di 1
    ovvero decrementa di 1 l'offset
*/
void Map::spostaVistaSinistra()
{
    this->_offset--;
    if (this->_offset < 0)
        this->_offset = 0;
    view_map = calcMappa();
}

/*
    Verifica se è possibile spostare la vista a sinistra,
    ovvero se la figura non collide o se non sono arrivato 
    all'inizio della mappa
*/
bool Map::possoSpostareVistaSinistra(ListaOggetto *listaObj, figura fig, int view)
{
    int id_coll = controllaCollisione(fig, -1, 0);
    bool solid = true;
    if (id_coll != -1)
        solid = listaObj->getDaId(id_coll)->getSolido();
    return ((id_coll == -1 || solid == false) && !dentroMargine(fig, -view, 0));
}

// PRIVATE

/*
    Aggiunge una riga alla riga passata come input, con valore y uguale a y parametro
*/
void Map::aggiungiRiga(riga *r, int y)
{

    // stack LIFO
    riga t = new riga_t;
    t->y = y;
    strcpy(t->c, "");
    t->id = -1;
    t->next = *r;
    *r = t;
}

/*
    Crea una nuova colonna alla mappa, e il numero della colonna è x
*/
void Map::creaColonna(int x)
{

    // stack LIFO
    mappa_t m = new colonna_t;
    m->x = x;
    m->r = NULL;
    m->next = map;
    map = m;
}

/*
    Aggiunge una colonna alla mappa e il numero della colonna è x, 
    inoltre crea tante righe per la colonna quanto è l'altezza della mappa
*/
void Map::aggiungiColonna(int x)
{
    creaColonna(x);
    for (int i = 0; i < _init_height; i++)
    {
        aggiungiRiga(&((map)->r), (_init_height - 1) - i);
    }
}

/*
    Verifica se le coordinate x, y sono all'interno dei margini della mappa
    se 0 <= x < _view
    se 0 <= y < _init_height
*/
bool Map::dentroMargine(int x, int y)
{
    bool dentro = true;

    if (x < 0 || x >= _view)
        dentro = false;
    if (y < 0 || y >= _init_height)
        dentro = false;

    return dentro;
}

bool Map::dentroMargine(figura fig)
{
    return dentroMargine(fig, 0, 0);
}

/*
    Verifica se ogni punto della figura incrementato di inc_x e inc_y
    rispettivamente per x e y, sono interni ai margini della mappa
*/
bool Map::dentroMargine(figura fig, int inc_x, int inc_y)
{
    bool dentro = true;

    while (fig != NULL && dentro == true)
    {
        dentro = dentroMargine(fig->x + inc_x, fig->y + inc_y);
        if (dentro == true)
            fig = fig->next;
    }

    return dentro;
}

bool Map::nemicoDentroMargine(figura fig)
{
    return nemicoDentroMargine(fig, 0, 0);
}

/*
    Verifica se ogni punto della figura incrementato di inc_x e inc_y
    rispettivamente per x e y, sono interni ai margini della mappa
*/
bool Map::nemicoDentroMargine(figura fig, int inc_x, int inc_y)
{
    bool dentro = true;

    while (fig != NULL && dentro == true)
    {
        bool dentroDestra = fig->x + inc_x <= (_view + _offset);
        bool dentroSinistra = fig->x + inc_x >= _offset;
        dentro = dentroSinistra && dentroDestra;
        if (dentro == true)
            fig = fig->next;
    }

    return dentro;
}
#include <fstream>
#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include "map.h"
#include "oggetto.h"

using namespace std;

// PUBLIC

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

void Map::initMap()
{
    map = new colonna_t;
    for (int i = 0; i < _init_width; i++)
    {
        aggiungiColonna(i);
    }
}

mappa_t Map::getMappa()
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

int Map::getWidth()
{
    return this->_width;
}

int Map::getHeight()
{
    return this->_init_height;
}

int Map::getView()
{
    return this->_view;
}

int Map::getOffset()
{
    return this->_offset;
}

void Map::setPunto(const char c[], int x, int y)
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
            strcpy(r->c, c);
    }
}

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

int Map::controllaCollisione(figura fig)
{
    return controllaCollisione(fig, 0, 0);
}

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

void Map::aggiungiOggetto(Oggetto obj)
{
    figura fig = obj.ottieniFigura();
    while (fig != NULL)
    {
        if (strlen(fig->c) > 0)
            setPunto(fig->c, fig->x, fig->y, obj.getId());
        fig = fig->next;
    }
}

void Map::rimuoviOggetto(Oggetto obj)
{
    figura fig = obj.ottieniFigura();
    while (fig != NULL)
    {
        if (strlen(fig->c) > 0)
            setPunto("", fig->x, fig->y, -1);
        fig = fig->next;
    }
}

void Map::spostaVistaDestra()
{
    this->_offset++;
    // aggiungo 5 colonne ovvero _width di gestore mondo
    for (int i = 0; i < 10; i++)
    {
        aggiungiColonna(_width);
        _width++;
    }
}

bool Map::possoSpostareVistaDestra(ListaOggetto *listaObj, figura fig, int view)
{
    int id_coll = controllaCollisione(fig, 1, 0);
    bool solid = true;
    if(id_coll != -1) solid = listaObj->getDaId(id_coll).getSolido();
    return ((id_coll == -1 || solid == false) && !dentroMargine(fig, view, 0));
}

void Map::spostaVistaSinistra()
{
    this->_offset--;
    if (this->_offset < 0)
        this->_offset = 0;
}

bool Map::possoSpostareVistaSinistra(ListaOggetto *listaObj, figura fig, int view)
{
    int id_coll = controllaCollisione(fig, -1, 0);
    bool solid = true;
    if(id_coll != -1) solid = listaObj->getDaId(id_coll).getSolido();
    return ((id_coll == -1 || solid == false) && !dentroMargine(fig, -view, 0));
}

// PRIVATE

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

void Map::creaColonna(int x)
{

    // stack LIFO
    mappa_t m = new colonna_t;
    m->x = x;
    m->r = NULL;
    m->next = map;
    map = m;
}

void Map::aggiungiColonna(int x)
{
    creaColonna(x);
    for (int i = 0; i < _init_height; i++)
    {
        aggiungiRiga(&((map)->r), (_init_height - 1) - i);
    }
}

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
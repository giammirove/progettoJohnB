#include <ncurses.h>
#include "map.h"
#include "oggetto.h"

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

int Map::controllaCollisione(figura fig) {
    return controllaCollisione(fig, 0, 0);
}

int Map::controllaCollisione(figura fig, int inc_x, int inc_y)
{
    bool found = false;
    int res = -1;
    while(fig != NULL && found == false) {
        int col = controllaCollisione(fig->x + inc_x, fig->y + inc_y);
        if(col != -1) {
            found = true;
            res = col;
        }else {
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
    aggiungiColonna(_width);
    _width++;
}

void Map::spostaVistaSinistra()
{
    this->_offset--;
    if (this->_offset < 0)
        this->_offset = 0;
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
    bool fuori = true;

    if (x < 0 || x >= _view)
        fuori = false;
    if (y < 0 || y >= _init_height)
        fuori = false;

    return fuori;
}
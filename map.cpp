#include "map.h"

Map::Map(int width, int height)
{
    _width = width;
    _height = height;

    initMap();
}

void Map::initMap()
{
    map = new colonna_t;
    for (int i = 0; i < _width; i++)
    {
        aggiungiColonna(&map, i);
        for (int j = 0; j < _height; j++)
        {
            // si inverte in quanto l'ordine qua ci interessa
            aggiungiRiga(&(map->r), (_height-1)-j);
        }
    }
}

mappa Map::getMappa()
{
    return map;
}

void Map::setPunto(const char c[], int x, int y)
{
    mappa m = this->map;
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

char * Map::getPunto(int x, int y)
{
    mappa m = this->map;
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
            return (r->c);
    }
}

void Map::aggiungiRiga(riga *r, int y)
{
    // stack LIFO
    riga t = new riga_t;
    t->y = y;
    strcpy(t->c, "");
    t->next = *r;
    *r = t;
}

void Map::aggiungiColonna(mappa *map, int x)
{
    // stack LIFO
    mappa m = new colonna_t;
    m->x = x;
    m->r = NULL;
    m->next = *map;
    *map = m;
}

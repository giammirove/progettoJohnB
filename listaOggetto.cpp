#include <string.h>
#include <ncurses.h>
#include "listaOggetto.h"
#include "oggetto.h"

ListaOggetto::ListaOggetto()
{
    _lista = NULL;
}

void ListaOggetto::aggiungi(Oggetto *obj)
{
    if (_lista == NULL)
    {
        _lista = new listaObj_t;
        _lista->obj = obj;
        _lista->next = NULL;
    }
    else
    {
        listaObj t = new listaObj_t;
        (t)->obj = obj;
        (t)->next = _lista;
        _lista = t;
    }
}

void ListaOggetto::rimuovi(Oggetto obj)
{
}

void ListaOggetto::rimuoviDaId(int id)
{
    listaObj t = _lista;
    bool rimosso = false;

    if (t != NULL)
    {
        if (t->obj->getId() == id)
        {
            _lista = t->next;
            delete t;
            rimosso = true;
        }
    }

    listaObj prev = NULL;
    while (t != NULL && rimosso == false)
    {
        if (t->obj->getId() == id)
        {
            listaObj n = t;
            if (prev != NULL)
                prev->next = t->next;
            else
                t = t->next;
            delete n;
            rimosso = true;
        }
        else
        {
            prev = t;
            t = t->next;
        }
    }
}

Oggetto ListaOggetto::getDaId(int id)
{
    listaObj t = _lista;
    while (t != NULL)
    {
        if (t->obj->getId() == id)
        {
            return *(t->obj);
        }
        else
        {
            t = t->next;
        }
    }

    return *(_lista->obj);
}

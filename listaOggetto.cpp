#include <string.h>
#include <ncurses.h>
#include "listaOggetto.h"
#include "oggetto.h"

/*
    Costrutture di ListaOggetto
    la lista viene inizializzata a NULL
*/
ListaOggetto::ListaOggetto()
{
    _lista = NULL;
}

/*
    geSize ritorna la lunghezza totale della lista
    scorrendola per intero
*/
int ListaOggetto::getSize()
{
    int s = 0;
    listaObj t = _lista;
    while (t != NULL)
    {
        s++;
        t = t->next;
    }
    return s;
}

/*
    Aggiunge un oggetto alla lista degli oggetti
*/
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

/*
    Rimuove un oggetto dalla lista
*/
void ListaOggetto::rimuovi(Oggetto obj)
{
}

/*
    Rimuove un oggetto dalla lista in base all'id
*/
void ListaOggetto::rimuoviDaId(int id)
{
    listaObj t = _lista;
    bool rimosso = false;

    // verifica la testa
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
        if (t->obj != NULL)
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
        else
        {
            prev = t;
            t = t->next;
        }
    }
}

/*
    Ottiene un oggetto in base al suo id
*/
Oggetto *ListaOggetto::getDaId(int id)
{
    listaObj t = _lista;
    while (t != NULL)
    {
        if (t->obj->getId() == id)
        {
            return (t->obj);
        }
        else
        {
            t = t->next;
        }
    }

    return NULL;
}

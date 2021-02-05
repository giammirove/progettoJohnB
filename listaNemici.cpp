#include <string.h>
#include <ncurses.h>
#include "listaNemici.h"
#include "nemico.h"

/*
    Costrutture di ListaNemici
    la lista viene inizializzata a NULL
*/
ListaNemici::ListaNemici()
{
    _lista = NULL;
}

/*
    geSize ritorna la lunghezza totale della lista
    scorrendola per intero
*/
int ListaNemici::getSize() {
    int s = 0;
    listaNemico t = _lista;
    while (t != NULL)
    {
        s++;
        t = t->next;
    }
    return s;
}

/*
    Aggiunge un Nemico alla lista degli oggetti
*/
void ListaNemici::aggiungi(Nemico *enm)
{
    if (_lista == NULL)
    {
        _lista = new listaNemico_t;
        _lista->enm = enm;
        _lista->next = NULL;
    }
    else
    {
        listaNemico t = new listaNemico_t;
        (t)->enm = enm;
        (t)->next = _lista;
        _lista = t;
    }
}

/*
    Rimuove un Nemico dalla lista
*/
void ListaNemici::rimuovi(Nemico enm)
{
}

/*
    Rimuove un Nemico dalla lista in base all'id
*/
void ListaNemici::rimuoviDaId(int id)
{
    listaNemico t = _lista;
    bool rimosso = false;

    if (t != NULL)
    {
        if (t->enm->getId() == id)
        {
            _lista = t->next;
            delete t;
            rimosso = true;
        }
    }

    listaNemico prev = NULL;
    while (t != NULL && rimosso == false)
    {
        if (t->enm->getId() == id)
        {
            listaNemico n = t;
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

/*
    Ottiene un Nemico in base al suo id
*/
Nemico ListaNemici::getDaId(int id)
{
    listaNemico t = _lista;
    while (t != NULL)
    {
        if (t->enm->getId() == id)
        {
            return *(t->enm);
        }
        else
        {
            t = t->next;
        }
    }

    return *(_lista->enm);
}

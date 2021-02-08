#include <string.h>
#include <ncurses.h>
#include "listaBonus.h"
#include "bonus.h"

/*
    Costrutture di ListaBonus
    la lista viene inizializzata a NULL
*/
ListaBonus::ListaBonus()
{
    _lista = NULL;
}

/*
    geSize ritorna la lunghezza totale della lista
    scorrendola per intero
*/
int ListaBonus::getSize() {
    int s = 0;
    listaBonus t = _lista;
    while (t != NULL)
    {
        s++;
        t = t->next;
    }
    return s;
}

/*
    Aggiunge un Bonus alla lista degli oggetti
*/
void ListaBonus::aggiungi(Bonus *nem)
{
    if (_lista == NULL)
    {
        _lista = new listaBonus_t;
        _lista->nem = nem;
        _lista->next = NULL;
    }
    else
    {
        listaBonus t = new listaBonus_t;
        (t)->nem = nem;
        (t)->next = _lista;
        _lista = t;
    }
}

/*
    Rimuove un Bonus dalla lista
*/
void ListaBonus::rimuovi(Bonus enm)
{
}

/*
    Rimuove un Bonus dalla lista in base all'id
*/
void ListaBonus::rimuoviDaId(int id)
{
    listaBonus t = _lista;
    bool rimosso = false;

    if (t != NULL)
    {
        if (t->nem->getId() == id)
        {
            _lista = t->next;
            delete t;
            rimosso = true;
        }
    }

    listaBonus prev = NULL;
    while (t != NULL && rimosso == false)
    {
        if (t->nem->getId() == id)
        {
            listaBonus n = t;
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
    Ottiene un Bonus in base al suo id
*/
Bonus *ListaBonus::getDaId(int id)
{
    listaBonus t = _lista;
    while (t != NULL)
    {
        if (t->nem->getId() == id)
        {
            return (t->nem);
        }
        else
        {
            t = t->next;
        }
    }

    return NULL;
}

/*
    Ottieni lista dei nemici
*/
listaBonus ListaBonus::getListaNemico() {
    return _lista;
}

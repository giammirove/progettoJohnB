#ifndef LISTABONUS_H_INCLUDED
#define LISTABONUS_H_INCLUDED

#include <string.h>
#include "bonus.h"

struct listaBonus_t
{
    Bonus *nem;
    listaBonus_t *next;
};
typedef struct listaBonus_t *listaBonus;

class ListaBonus
{
private:
    listaBonus _lista;

public:
    ListaBonus();

    int getSize();

    void aggiungi(Bonus *nem);
    void rimuovi(Bonus nem);
    void rimuoviDaId(int id);
    Bonus *getDaId(int id);
    listaBonus getListaNemico();
};

#endif
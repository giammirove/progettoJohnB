#ifndef LISTANEMICI_H_INCLUDED
#define LISTANEMICI_H_INCLUDED

#include <string.h>
#include "nemico.h"

struct listaNemico_t
{
    Nemico *nem;
    listaNemico_t *next;
};
typedef struct listaNemico_t *listaNemico;

class ListaNemici
{
private:
    listaNemico _lista;

public:
    ListaNemici();

    int getSize();

    void aggiungi(Nemico *nem);
    void rimuovi(Nemico nem);
    void rimuoviDaId(int id);
    Nemico *getDaId(int id);
    listaNemico getListaNemico();
};

#endif
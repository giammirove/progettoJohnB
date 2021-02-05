#ifndef LISTANEMICI_H_INCLUDED
#define LISTANEMICI_H_INCLUDED

#include <string.h>
#include "nemico.h"

struct listaNemico_t
{
    Nemico *enm;
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

    void aggiungi(Nemico *enm);
    void rimuovi(Nemico enm);
    void rimuoviDaId(int id);
    Nemico getDaId(int id);
};

#endif
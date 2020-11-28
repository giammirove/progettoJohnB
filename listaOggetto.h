#ifndef LISTAOGGETTO_H_INCLUDED
#define LISTAOGGETTO_H_INCLUDED

#include <string.h>
#include "oggetto.h"

struct listaObj_t
{
    Oggetto *obj;
    listaObj_t *next;
};
typedef struct listaObj_t *listaObj;

class ListaOggetto
{
private:
    listaObj _lista;

public:
    ListaOggetto();

    void aggiungi(Oggetto *obj);
    void rimuovi(Oggetto obj);
    void rimuoviDaId(int id);
    Oggetto getDaId(int id);
};

#endif
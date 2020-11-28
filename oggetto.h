#ifndef OGGETTO_H_INCLUDED
#define OGGETTO_H_INCLUDED

#include <string.h>

struct figura_t
{
    int x;
    int y;
    char c[30];
    figura_t *next;
};
typedef struct figura_t *figura;

void aggiungiPuntoAFigura(figura *fig, int x, int y, const char c[]);

enum TipoDiOggetto
{
    OS_ALBERO,
    OS_CESPUGLIO,
    OS_KIRBY,
    OS_PIATTAFORMA
};

class Oggetto
{

public:
    static int _id;

    Oggetto(int x, int y, TipoDiOggetto tipo);
    int getId();
    int getClock();
    bool getSolido();
    figura ottieniFigura();
    TipoDiOggetto getTipoDiOggetto();

private:
    int _x, _y;
    int _myId;
    int _clock;
    bool _solido;
    figura _figura;
    TipoDiOggetto _tipo;

    void impostaFigura(TipoDiOggetto tipo);
};

#endif
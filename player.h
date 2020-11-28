#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <string.h>
#include "oggetto.h"

class Player{
    private : 
        int _x;
        int _y;
        int _view;
        int _clock;
        bool _gravita;
        figura _figura;
        char _c[30];

        void impostaFigura();
        void aggiornaFigura(int inc_x, int inc_y);

    public :
        Player(int x, int y, int view);

        int getX();
        int getY();
        int getView();
        int getClock();
        bool getGravita();
        figura getFigura();
        char *getChar();

        void vaiADestra();

        void vaiASinistra();

        void vaiInAlto();

        void vaiInBasso();
};

#endif
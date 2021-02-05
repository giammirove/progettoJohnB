#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <string.h>
#include "oggetto.h"

class Player
{
private:
    int _score;
    int _vita;
    const int _MAX_VITA = 10;
    const int _MOV_LATERALE_IN_ARIA = 2;
    int _x;
    int _y;
    int _width;
    int _heigth;
    int _view;
    int _clock;
    int _saltaHeight;
    int _saltaInt;
    bool _saltaDestra;
    bool _saltaSinistra;
    bool _gravita;
    bool _aterra;
    figura _figura;

    void aggiornaFigura(int inc_x, int inc_y);

public:
    Player(int x, int y, int view, int saltaHeight);
    Player(int x, int y, int view, int saltaHeight, figura fig);

    int getScore();
    int getVita();
    int getX();
    int getY();
    int getWidth();
    int getHeight();
    int getView();
    int getClock();
    bool getGravita();
    int getSaltaHeight();
    int getSaltaInt();
    bool getSaltaDestra();
    bool getSaltaSinistra();

    bool getATerra();
    bool setATerra(bool val);
    bool stoSaltando();
    bool sonoInAriaDuranteIlSalto();
    bool stoScendendo();

    figura getFigura();
    void setFigura(figura fig);

    void vaiADestra();

    void vaiASinistra();

    void vaiInAlto();

    void vaiInBasso();

    void salta();
    void resettaSalto();
    void decrementaSalto();
    void saltaDestra();
    void saltaSinistra();
    void resettaSaltoDestraSinistra();

    int decrementaScore(int n);
    int decrementaScore();
    int incrementaScore(int n);
    int incrementaScore();

    int decrementaVita(int n);
    int decrementaVita();
    int incrementaVita(int n);
    int incrementaVita();
};

#endif
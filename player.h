#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <string.h>
#include "oggetto.h"
#include "convertiAsciiArt.h"
#include "weapon.h"

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
    int _height;
    int _view;
    int _clock;
    int _saltaHeight;
    int _saltaInt;
    bool _saltaDestra;
    bool _saltaSinistra;
    bool _gravita;
    bool _aterra;
    bool _direzione; // true se a sinistra, false se a destra
    figura _figura;
    Weapon *_arma;
    int _armaAttiva;
    int _bonusSalto;
    int _invulnerabile;
	int _MAX_INVULNERABILE;

    void aggiornaFigura(int inc_x, int inc_y);
    void calcolaPosizioneArma(int *pos_x, int *pos_y);

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
    bool getDirezione();

    bool getInvulnerabile();
    int getValoreInvulnerabile();
	int decrementaInvulnerabile();
	bool setInvulnerabile();

    bool toccoLaLava(int map_h);

    bool getATerra();
    bool setATerra(bool val);
    bool stoSaltando();
    bool sonoInAriaDuranteIlSalto();
    bool stoScendendo();

    figura getFigura();
    void setFigura(figura fig);
    void calcWidthAndHeight();

    Weapon *getArma();
    void setArma(TipoDiOggetto tipoArma, ConvertiAsciiArt *asciiArt);
    int cambiaArmaAttiva(int val);
    int getValoreArma();
    bool decrementaArmaAttiva();
    bool getArmaAttiva();

    int incrementaBonusSalto(int val);
    int incrementaBonusSalto();
    int decrementaBonusSalto(int val);
    int decrementaBonusSalto();
    int getBonusSalto();
    bool possiedoBonusSalto();

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
    int muori();
};

#endif
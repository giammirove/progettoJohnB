#include <string.h>
#include "player.h"
#include "oggetto.h"

Player::Player(int x, int y, int view)
{
    _x = x;
    _y = y;
    _view = view;
    _clock = 1000;
    _gravita = true;

    impostaFigura();
}

int Player::getX(){
    return _x;
}

int Player::getY(){
    return _y;
}

int Player::getView(){
    return _view;
}

int Player::getClock(){
    return _clock;
}

bool Player::getGravita() {
    return _gravita;
}

figura Player::getFigura(){
    return _figura;
}

char * Player::getChar(){
    return _c;
}

void Player::vaiADestra()
{
    _x++;
    aggiornaFigura(1, 0);
}

void Player::vaiASinistra()
{
    _x--;
    aggiornaFigura(-1, 0);
}

void Player::vaiInAlto()
{
    _y--;
    aggiornaFigura(0, -1);
}

void Player::vaiInBasso()
{
    _y++;
    aggiornaFigura(0, 1);
}



// PRIVATE

void Player::impostaFigura() {
    aggiungiPuntoAFigura(&_figura, _x, _y, "■");
    aggiungiPuntoAFigura(&_figura, _x+1, _y, "■");
    aggiungiPuntoAFigura(&_figura, _x+2, _y, "■");
    aggiungiPuntoAFigura(&_figura, _x+1, _y+1, "■");
}


void Player::aggiornaFigura(int inc_x, int inc_y) {
    figura t = _figura;
    while(t != NULL) {
        t->x = t->x + inc_x;
        t->y = t->y + inc_y;
        t = t->next;
    }
}
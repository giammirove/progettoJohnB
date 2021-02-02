#include <string.h>
#include <iostream>
#include <fstream>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include "player.h"
#include "oggetto.h"

using namespace std;

/*
    Costruttore della classe Player 
    x , y = sono le coordinate del player
    view = è la vista del player
    saltaHeight = è la capacità di salto del player
*/
Player::Player(int x, int y, int view, int saltaHeight)
{
    _x = x;
    _y = y;
    _view = view;
    _clock = 500;
    _gravita = true;
    _saltaHeight = saltaHeight;
    resettaSalto();
}

/*
    Costruttore del Player
    con aggiunta della figura per impostarla direttamente
*/
Player::Player(int x, int y, int view, int saltaHeight, figura fig)
{
    _x = x;
    _y = y;
    _view = view;
    _clock = 500;
    _gravita = true;
    _saltaHeight = saltaHeight;
    resettaSalto();

    setFigura(fig);
}

/*
    Ottiene la coordinata x
*/
int Player::getX()
{
    return _x;
}

/*
    Ottiene la coordinata y
*/
int Player::getY()
{
    return _y;
}

/*
    Ritorna la lunghezza
*/
int Player::getWidth()
{
    return _width;
}

/*
    Ritorna l'altezza
*/
int Player::getHeight()
{
    return _heigth;
}

/*
    Ritorna la vista
*/
int Player::getView()
{
    return _view;
}

/*
    Ritorna il clock del player
*/
int Player::getClock()
{
    return _clock;
}

/*
    Ritorna se il player è soggetto a gravita
*/
bool Player::getGravita()
{
    return _gravita;
}

/*
    Ritorna la capacità di salto del player
*/
int Player::getSaltaHeight() {
    return _saltaHeight;
}

/*
    Ritorna lo stato del salto del player
    si comprende meglio il suo significa guardando
    la funzione che gestisce la gravità
*/
int Player::getSaltaInt()
{
    return _saltaInt;
}

/*
    Ritorna se il player sta saltando a destra
*/
bool Player::getSaltaDestra() {
    return _saltaDestra;
}

/*
    Ritorna se il player sta saltando a sinistra
*/
bool Player::getSaltaSinistra() {
    return _saltaSinistra;
}

/*
    Ritorna la figura corrente del player
*/
figura Player::getFigura()
{
    return _figura;
}

/*
    Imposta la figura del player con passaggio per parametro di essa
*/
void Player::setFigura(figura fig)
{
    if (_figura != fig)
    {
        // elimino la figura precedente
        if (_figura != NULL)
            delete _figura;
        _width = 0;
        _heigth = 0;
        _figura = NULL;
        figura t = fig;
        while (t != NULL)
        {
            // permette di calcolare la lunghezza
            if (_width < t->x)
                _width = t->x;
            // permette di calcolare l'altezza
            if (_heigth < t->y)
                _heigth = t->y;
            // viene aggiunto punto per punto alla figura del player
            aggiungiPuntoAFigura(&_figura, t->x + _x, t->y + _y, t->c);
            t = t->next;
        }
    }
}

/*
    Muovo il player a destra di 1
*/
void Player::vaiADestra()
{
    _x++;
    aggiornaFigura(1, 0);
}

/*
    Muovo il player a sinistra di 1
*/
void Player::vaiASinistra()
{
    _x--;
    aggiornaFigura(-1, 0);
}

/*
    Muovo il player in alto di 1
*/
void Player::vaiInAlto()
{
    _y--;
    aggiornaFigura(0, -1);
}

/*
    Muovo il player in basso di 1
*/
void Player::vaiInBasso()
{
    _y++;
    aggiornaFigura(0, 1);
}

/*
    Imposto che il player debba saltare, quindi assegno a saltaInt il valore 
    della capacità di salto, saltaInt verrà poi decrementato
*/
void Player::salta()
{
    if (_saltaInt == 0)
        _saltaInt = _saltaHeight;
}

/*
    Riduco saltaInt di una unità se maggiore di 0
    Altrimenti ho terminato di saltare e posso resettare i flag
*/
void Player::decrementaSalto()
{
    if (_saltaInt > 0)
        _saltaInt--;
    else {
        resettaSaltoDestraSinistra();
    }
}

/*
    Ho terminato di saltare quindi saltaInt = 0
    e resetto i flag
*/
void Player::resettaSalto() {
    _saltaInt = 0;
    resettaSaltoDestraSinistra();
}

/*
    Imposto che il player sta saltando a destra
*/
void Player::saltaDestra () {
    _saltaDestra = true;
}

/*
    Imposto che il player sta saltando a sinistra
*/
void Player::saltaSinistra () {
    _saltaSinistra = true;
}

/*
    Resetto i flag che avvertono se il player sta saltando
    a destra o a sinistra
*/
void Player::resettaSaltoDestraSinistra(){
    _saltaSinistra = false;
    _saltaDestra = false;
}

// PRIVATE

/*
    Modifico ogni punto della figura corrente 
    di inc_x per le x e di inc_y per le y
*/
void Player::aggiornaFigura(int inc_x, int inc_y)
{
    figura t = _figura;
    while (t != NULL)
    {
        t->x = t->x + inc_x;
        t->y = t->y + inc_y;
        t = t->next;
    }
}
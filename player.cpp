#include <string.h>
#include <iostream>
#include <fstream>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include "player.h"
#include "oggetto.h"

using namespace std;

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

int Player::getX()
{
    return _x;
}

int Player::getY()
{
    return _y;
}

int Player::getWidth()
{
    return _width;
}

int Player::getHeight()
{
    return _heigth;
}

int Player::getView()
{
    return _view;
}

int Player::getClock()
{
    return _clock;
}

bool Player::getGravita()
{
    return _gravita;
}

int Player::getSaltaHeight() {
    return _saltaHeight;
}

int Player::getSaltaInt()
{
    return _saltaInt;
}

bool Player::getSaltaDestra() {
    return _saltaDestra;
}

bool Player::getSaltaSinistra() {
    return _saltaSinistra;
}

figura Player::getFigura()
{
    return _figura;
}

void Player::setFigura(figura fig)
{
    if (_figura != fig)
    {
        if (_figura != NULL)
            delete _figura;
        _width = 0;
        _heigth = 0;
        _figura = NULL;
        figura t = fig;
        while (t != NULL)
        {
            if (_width < t->x)
                _width = t->x;
            if (_heigth < t->y)
                _heigth = t->y;
            aggiungiPuntoAFigura(&_figura, t->x + _x, t->y + _y, t->c);
            t = t->next;
        }
    }
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

void Player::salta()
{
    if (_saltaInt == 0)
        _saltaInt = _saltaHeight;
}

void Player::decrementaSalto()
{
    if (_saltaInt > 0)
        _saltaInt--;
    else {
        resettaSaltoDestraSinistra();
    }
}

void Player::resettaSalto() {
    _saltaInt = 0;
    resettaSaltoDestraSinistra();
}

void Player::saltaDestra () {
    _saltaDestra = true;
}

void Player::saltaSinistra () {
    _saltaSinistra = true;
}

void Player::resettaSaltoDestraSinistra(){
    _saltaSinistra = false;
    _saltaDestra = false;
}

// PRIVATE

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
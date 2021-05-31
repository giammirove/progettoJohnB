#include <string.h>
#include <iostream>
#include <fstream>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include "player.h"
#include "weapon.h"
#include "convertiAsciiArt.h"
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
    _aterra = true;
    _saltaHeight = saltaHeight;
    _score = 0;
    _vita = 3;
    _armaAttiva = false;
    _invulnerabile = 0;
    _MAX_INVULNERABILE = 1000;
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
    _clock = 100;
    _gravita = true;
    _aterra = true;
    _saltaHeight = saltaHeight;
    _score = 0;
    _vita = 3;
    _armaAttiva = false;
    _invulnerabile = 0;
    _MAX_INVULNERABILE = 30;
    resettaSalto();

    setFigura(fig);
}

/*
    Ottiene lo score del player
*/
int Player::getScore()
{
    return _score;
}

/*
    Ottiene la vita del player
*/
int Player::getVita()
{
    return _vita;
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
    return _height;
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
    Ritorna se il player si trova a terra
*/
bool Player::getATerra()
{
    return _aterra;
}

/*
    Imposta se il player si trova a terra
*/
bool Player::setATerra(bool val)
{
    _aterra = val;
    return _aterra;
}

bool Player::stoSaltando()
{
    return (getSaltaInt() > (getSaltaHeight() - _MOV_LATERALE_IN_ARIA) / 2 + _MOV_LATERALE_IN_ARIA);
}

bool Player::sonoInAriaDuranteIlSalto()
{
    return (getSaltaInt() >= ((getSaltaHeight() - _MOV_LATERALE_IN_ARIA) / 2) && getSaltaInt() <= ((getSaltaHeight() - _MOV_LATERALE_IN_ARIA) / 2 + _MOV_LATERALE_IN_ARIA));
}

bool Player::stoScendendo()
{
    return (getSaltaInt() < ((getSaltaHeight() - _MOV_LATERALE_IN_ARIA) / 2));
}

/*
    Ritorna la capacità di salto del player
*/
int Player::getSaltaHeight()
{
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
bool Player::getSaltaDestra()
{
    return _saltaDestra;
}

/*
    Ritorna se il player sta saltando a sinistra
*/
bool Player::getSaltaSinistra()
{
    return _saltaSinistra;
}

/*
    Ritorna true sse il player guarda a sinistra
    false sse il player guarda a destra
*/
bool Player::getDirezione()
{
    return _direzione;
}

/*
	Ritorna se il player è invulnerabile
*/
bool Player::getInvulnerabile()
{
    return _invulnerabile > 0;
}

/*
	Ritorna il numero di riferimento per l'invulnerabilità 
*/
int Player::getValoreInvulnerabile()
{
    return _invulnerabile;
}

/*
	Decrementa la variabile dell'incremento
*/
int Player::decrementaInvulnerabile()
{
    _invulnerabile--;
    if (_invulnerabile < 0)
        _invulnerabile = 0;
    return _invulnerabile;
}

/*
	Cambia lo stato di invulnerabile in val
*/
bool Player::setInvulnerabile()
{
    _invulnerabile = _MAX_INVULNERABILE;
    return getInvulnerabile();
}

/*
    Verifica se sta toccando la lava
*/
bool Player::toccoLaLava(int map_h)
{
    figura f = _figura;
    while (f->next != NULL)
    {
        f = f->next;
    }
    if (f != NULL)
    {
        if (f->y == map_h - 1)
            return true;
    }
    return false;
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
        _figura = NULL;
        figura t = fig;
        while (t != NULL)
        {
            // viene aggiunto punto per punto alla figura del player
            aggiungiPuntoAFigura(&_figura, t->x + _x, t->y + _y, t->c);
            t = t->next;
        }
    }
    calcWidthAndHeight();
}

/*
    Calcola la dimensione della figura, sia lunghezza che larghezza 
    e salva i risultati rispettivamente nelle variabili 
    _width e _height

    Spiegazione algoritmo :
    si salva inizialmente la minima x e la minima y
    e la massima x e la massima y
    controlla ogni punto della figura e verifica se 
    i minimi/massimi attuali sono da aggiornare
    la lunghezza calcolata sarà la differenza tra 
    il massimo x e il minimo x
    la altezza calcolata sarà la differenza tra 
    il massimo y e il minimo y
*/
void Player::calcWidthAndHeight()
{
    if (_figura != NULL)
    {
        int min_w = 1;
        int max_w = _figura->x - _x;
        int min_h = 1;
        int max_h = _figura->y - _y;
        figura tmp = _figura;
        while (tmp != NULL)
        {
            if (tmp->x - _x > max_w)
            {
                max_w = tmp->x - _x;
            }
            if (tmp->x - _x < min_w)
            {
                min_w = tmp->x - _x;
            }
            if (tmp->y - _y > max_h)
            {
                max_h = tmp->y - _y;
            }
            if (tmp->y - _y < min_h)
            {
                min_h = tmp->y - _y;
            }

            tmp = tmp->next;
        }

        _width = abs(max_w);
        _height = abs(max_h);
    }
}

/*
    Ritorna l'oggetto arma del player
*/
Weapon *Player::getArma()
{
    return _arma;
}

/*
    Imposta l'arma del player
*/
void Player::setArma(TipoDiOggetto tipoArma, ConvertiAsciiArt *asciiArt)
{
    int arma_x = 0;
    int arma_y = 0;
    calcolaPosizioneArma(&arma_x, &arma_y);
    _arma = new Weapon(0, 0, tipoArma, asciiArt);
    // sto guardando a sinistra
    if (_direzione == true)
    {
        _arma->muoviFigura(arma_x - _arma->getWidth(), arma_y);
    }
    // sto andando a destra
    else
    {
        _arma->muoviFigura(arma_x, arma_y);
    }
}

/*
    Cambia lo stato di visibilità dell'arma
*/
int Player::cambiaArmaAttiva(int val)
{
    _armaAttiva += val;
    return _armaAttiva;
}

/*
    Ritorna lo stato di visibilità dell'arma
*/
int Player::getValoreArma(){
    return _armaAttiva;
}

/*
    Decrementa lo stato di attivo dell'arma
*/
bool Player::decrementaArmaAttiva() {
    _armaAttiva--;
    if(_armaAttiva < 0) _armaAttiva = 0;
    return getArmaAttiva();
}

/*
    Ritorna lo stato di visibilità dell'arma
*/
bool Player::getArmaAttiva()
{
    return _armaAttiva > 0;
}

/*
    Muovo il player a destra di 1
*/
void Player::vaiADestra()
{
    _x++;
    _direzione = false;
    aggiornaFigura(1, 0);
}

/*
    Muovo il player a sinistra di 1
*/
void Player::vaiASinistra()
{
    _x--;
    _direzione = true;
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
    else
    {
        resettaSaltoDestraSinistra();
    }
}

/*
    Ho terminato di saltare quindi saltaInt = 0
    e resetto i flag
*/
void Player::resettaSalto()
{
    _saltaInt = 0;
    resettaSaltoDestraSinistra();
}

/*
    Imposto che il player sta saltando a destra
*/
void Player::saltaDestra()
{
    _saltaDestra = true;
}

/*
    Imposto che il player sta saltando a sinistra
*/
void Player::saltaSinistra()
{
    _saltaSinistra = true;
}

/*
    Resetto i flag che avvertono se il player sta saltando
    a destra o a sinistra
*/
void Player::resettaSaltoDestraSinistra()
{
    _saltaSinistra = false;
    _saltaDestra = false;
}

/*
    Decrementa la vita del player di n
*/
int Player::decrementaVita(int n)
{
    if (getInvulnerabile() == false)
    {
        setInvulnerabile();
        _vita -= n;
        if (_vita < 0)
            _vita = 0;
    }
    return _vita;
}

/*
    Decrementa la vita del player di 1
*/
int Player::decrementaVita()
{
    return decrementaVita(1);
}

/*
    Incrementa la vita del player di n 
*/
int Player::incrementaVita(int n)
{
    _vita += n;
    if (_vita > _MAX_VITA)
        _vita = _MAX_VITA;
    return _vita;
}

/*
    Incrementa la vita del player di 1
*/
int Player::incrementaVita()
{
    return incrementaVita(1);
}

/*
    Decrementa lo score del player di n
*/
int Player::decrementaScore(int n)
{
    _score -= n;
    if (_score < 0)
        _score = 0;
    return _score;
}

/*
    Decrementa lo score del player di 1
*/
int Player::decrementaScore()
{
    return decrementaScore(1);
}

/*
    Incrementa lo score del player di n 
*/
int Player::incrementaScore(int n)
{
    _score += n;
    return _score;
}

/*
    Incrementa lo score del player di 1
*/
int Player::incrementaScore()
{
    return incrementaScore(1);
}

/*
    Muore all'istante
*/
int Player::muori()
{
    _vita = 0;
    return _vita;
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

    // se la direzione precedente è diversa aggiorno l'immagine
    if (_direzione != _arma->getDirezione())
    {
        _arma->setDirezione(_direzione);
        if (_direzione)
            _arma->muoviFigura(-(_width * 2), 0);
        else
            _arma->muoviFigura((_width * 2), 0);
        //mvprintw(3, 100, "WIDTH %d", _width);
    }
    _arma->muoviFigura(inc_x, inc_y);
    //mvprintw(2, 100, "%d", _arma->getFigura()->x);
}

/*
    Calcola la posizione dell'arma rispetto al player
*/
void Player::calcolaPosizioneArma(int *pos_x, int *pos_y)
{
    *pos_x = _figura->x;
    *pos_y = _figura->y;
    figura _tmp = _figura;
    if (_tmp != NULL)
    {
        int max_x = _tmp->x;
        int min_x = _tmp->x;
        int max_y = _tmp->y;
        int min_y = _tmp->y;
        while (_tmp != NULL)
        {
            if (_tmp->x > max_x)
            {
                max_x = _tmp->x;
            }
            if (_tmp->x < min_x)
            {
                min_x = _tmp->x;
            }
            if (_tmp->y > max_y)
            {
                max_y = _tmp->y;
            }
            if (_tmp->y < min_y)
            {
                min_y = _tmp->y;
            }
            _tmp = _tmp->next;
        }

        int media_y = (int)((max_y + min_y) / 2);
        *pos_y = media_y;

        // sto guardando a sinistra
        if (_direzione == true)
        {
            *pos_x = min_x - 1;
        }
        // sto guardando a destra
        else
        {
            *pos_x = max_x + 1;
        }
    }
}
#include <string.h>
#include <locale.h>
#include <ncurses.h>
#include "oggetto.h"
#include "convertiAsciiArt.h"
#include "utility.h"

/*
    Id è una variabile statica che si incrementa ad ogni 
    invocazione del costruttore della classe Oggetto
*/
int Oggetto::_id = 0;

/*
    Costruttore della classe Oggetto
    x = posizione x dell'oggetto
    y = posizione y dell'oggetto
    tipo = indicato il tipo di oggetto creato (guardate struttura nel file oggetto.h)
    asciiArt = si tratta di un classe che permette di ottenere un oggetto figura a partire
               dal file asciiArtDB.txt
*/
Oggetto::Oggetto(int x, int y, TipoDiOggetto tipo, ConvertiAsciiArt *asciiArt)
{
    _x = x;
    _y = y;
    _solido = false;

    impostaFigura(tipo, asciiArt);
    _myId = _id++;
}

/*
    Ottiene l'id dell'oggetto
    L'id è un valore univoco di ogni oggetto
*/
int Oggetto::getId()
{
    return _myId;
}

/*
    Ottiene il clock di questo oggetto
    Ogni oggetto ha un clock differente legato al tipo dell'oggetto
*/
int Oggetto::getClock()
{
    return _clock;
}

/*
    Ottiene un valore booleano che indica se il tipo di oggetto
    è solido oppure no
    Con solido si intende se il player può attraversare l'oggetto
    da sinistra, destra e dal basso (es. di non solido = piattaforma standard)
*/
bool Oggetto::getSolido()
{
    return _solido;
}

/*  
    Ottiene la lunghezza dell'oggetto
    Calcolata in base alla figura dell'oggetto
*/
int Oggetto::getWidth()
{
    return _width;
}

/*
    Ottiene la altezza dell'oggetto
    Calcolata in base alla figura dell'oggetto
*/
int Oggetto::getHeight()
{
    return _height;
}

/*
    Ottiene il tipo di oggetto
*/
TipoDiOggetto Oggetto::getTipoDiOggetto()
{
    return _tipo;
}

/*
    Imposta la figura dell'oggetto in base al tipo
    tipo = indica il tipo di oggetto, in base ad esso si sceglie la figura 
    asciiArt = oggetto che permette di ottenere la figura 
*/
void Oggetto::impostaFigura(TipoDiOggetto tipo, ConvertiAsciiArt *asciiArt)
{
    _figura = new figura_t;
    _tipo = tipo;

    switch (tipo)
    {
    case OS_ALBERO:
        aggiungiPuntoAFigura(&_figura, _x, _y, "\U0001F333");
        aggiungiPuntoAFigura(&_figura, _x + 1, _y, " ");
        _clock = 100000;
        _solido = true;
        break;
    case OS_CESPUGLIO:
        aggiungiPuntoAFigura(&_figura, _x, _y, "o");
        //aggiungiPuntoAFigura(&_figura, _x+1, _y, " ");
        _clock = 100000;
        _solido = true;
        break;
    case OS_KIRBY:
        aggiungiPuntoAFigura(&_figura, _x, _y, "\U0001F416");
        aggiungiPuntoAFigura(&_figura, _x + 1, _y, " ");
        _clock = 100000;
        _solido = true;
        break;
    case OS_PIATTAFORMA:
        aggiungiPuntoAFigura(&_figura, _x, _y, "=");
        aggiungiPuntoAFigura(&_figura, _x + 1, _y, "=");
        aggiungiPuntoAFigura(&_figura, _x + 2, _y, "=");
        _clock = 100000;
        _solido = false;
        break;
    case OS_PIATTAFORMA2:
        aggiungiPuntoAFigura(&_figura, _x, _y, "=");
        aggiungiPuntoAFigura(&_figura, _x + 1, _y, "=");
        aggiungiPuntoAFigura(&_figura, _x + 2, _y, "=");
        aggiungiPuntoAFigura(&_figura, _x + 3, _y, "=");
        aggiungiPuntoAFigura(&_figura, _x + 4, _y, "=");
        _clock = 100000;
        _solido = false;
        break;
    case OS_WINDMILL:
        caricaFigura("WINDMILL", asciiArt);
        _clock = 100000;
        _solido = true;
        break;
    }
    calcWidthAndHeight();
}

/*
    Carica la figura in base al nome di essa
    nome = indica il nome della figura all'interno del file "asciiArtDB.txt"
    asciiArt = oggetto che permette di ottenere la figura
*/
void Oggetto::caricaFigura(char *nome, ConvertiAsciiArt *asciiArt)
{
    if (_figura != NULL)
        delete _figura;
    _figura = NULL;
    figura t = asciiArt->getFigura(nome);
    while (t != NULL)
    {
        aggiungiPuntoAFigura(&_figura, t->x + _x, t->y + _y, t->c);
        t = t->next;
    }
}

/*
    Ottiene la figura dell'oggeto
*/
figura Oggetto::ottieniFigura()
{
    return _figura;
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
void Oggetto::calcWidthAndHeight()
{
    if (_figura != NULL)
    {
        int min_w = _figura->x;
        int max_w = _figura->x;
        int min_h = _figura->y;
        int max_h = _figura->y;
        figura tmp = _figura;
        while (tmp != NULL)
        {
            if (tmp->x > max_w)
            {
                max_w = tmp->x;
            }
            if (tmp->x < min_w)
            {
                max_w = tmp->x;
            }
            if (tmp->y > max_h)
            {
                max_h = tmp->y;
            }
            if (tmp->y < min_h)
            {
                max_h = tmp->y;
            }

            tmp = tmp->next;
        }

        _width = max_w - min_w;
        _height = max_h - min_h;
    }
}

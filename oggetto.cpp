#include <string.h>
#include <locale.h>
#include <ncurses.h>
#include <math.h>
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
    if (_figura != NULL) delete _figura;
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
        for (int i = 0; i < 10; i++)
        {
            aggiungiPuntoAFigura(&_figura, _x + i, _y, "=");
        }
        _clock = 100000;
        _solido = false;
        break;
    case OS_PIATTAFORMA2:
        for (int i = 0; i < 20; i++)
        {
            aggiungiPuntoAFigura(&_figura, _x + i, _y, "=");
        }
        _clock = 100000;
        _solido = false;
        break;
    case OS_PAVIMENTO:
        aggiungiPuntoAFigura(&_figura, _x, _y, "-");
        aggiungiPuntoAFigura(&_figura, _x + 1, _y, "-");
        aggiungiPuntoAFigura(&_figura, _x + 2, _y, "-");
        aggiungiPuntoAFigura(&_figura, _x + 3, _y, "-");
        aggiungiPuntoAFigura(&_figura, _x + 4, _y, "-");
        aggiungiPuntoAFigura(&_figura, _x + 5, _y, "-");
        _clock = 100000;
        _solido = true;
        break;
    case OS_WINDMILL:
        caricaFigura(&_figura, "WINDMILL", asciiArt);
        _clock = 100000;
        _solido = true;
        break;
    case OS_NEMICO1:
        caricaFigura(&_figura, "NEMICO1", asciiArt);
        _clock = 100000;
        _solido = false;
        break;
    case OS_NEMICO2:
        caricaFigura(&_figura, "NEMICO2", asciiArt);
        _clock = 100000;
        _solido = false;
        break;
    case OS_ARMA1:
        caricaFigura(&_figura, "ARMA1_DX", asciiArt);
        _clock = 100000;
        _solido = false;
        break;
    default:
        aggiungiPuntoAFigura(&_figura, _x, _y, "?");
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
void Oggetto::caricaFigura(figura *fig, char *nome, ConvertiAsciiArt *asciiArt)
{
    if (*fig != NULL)
        delete *fig;
    *fig = NULL;
    figura t = asciiArt->getFigura(nome);
    while (t != NULL)
    {
        aggiungiPuntoAFigura(fig, t->x + _x, t->y + _y, t->c);
        t = t->next;
    }
}

void Oggetto::caricaFigura(figura *fig, figura src) {
    if (*fig != NULL)
        delete *fig;
    *fig = NULL;
    while(src != NULL) {
        aggiungiPuntoAFigura(fig, src->x + _x, src->y + _y, src->c);
        src = src->next;
    }
}

/*
    Ottiene la figura dell'oggeto
*/
figura Oggetto::getFigura()
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
    Muove la figura con incremento in x e y per ogni punto della figura
    di inc_x e inc_y
*/
figura Oggetto::muoviFigura(int inc_x, int inc_y)
{
    figura tmp = _figura;
    figura head = tmp;
    while (tmp != NULL)
    {
        tmp->x += inc_x;
        tmp->y += inc_y;
        tmp = tmp->next;
    }
    _figura = head;
    return _figura;
}
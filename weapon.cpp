#include "weapon.h"
#include "utility.h"

Weapon::Weapon(int x, int y, TipoDiOggetto tipo, ConvertiAsciiArt *asciiArt) : Oggetto(x, y, tipo, asciiArt)
{
    // default a destra
    _direzione = false;
    _figuraSx = NULL;
    _figuraDx = NULL;

    impostaFigura(tipo, asciiArt);
}

bool Weapon::hit(Map *mappa)
{
    int buff_id = mappa->controllaCollisione(getFigura());
    return true;
}

void Weapon::impostaFigura(TipoDiOggetto tipo, ConvertiAsciiArt *asciiArt)
{   
    Oggetto::impostaFigura(tipo, asciiArt);
    switch (tipo)
    {

    case OS_ARMA1:
        _danno = 1;
        _direzione = false;
        caricaFigura(&_figuraDx, "ARMA1_DX", asciiArt);
        caricaFigura(&_figuraSx, "ARMA1_SX", asciiArt);
        //caricaFigura(&_figura, "ARMA1_DX", asciiArt);
        break;

    default:
        break;
    }
}

figura Weapon::getFigura(){
    if(_direzione) return _figuraSx;
    return _figuraDx;
}

/*
    Muove la figura con incremento in x e y per ogni punto della figura
    di inc_x e inc_y
*/
figura Weapon::muoviFigura(int inc_x, int inc_y)
{
    figura tmp = _figuraDx;
    figura head = tmp;
    while (tmp != NULL)
    {
        tmp->x += inc_x;
        tmp->y += inc_y;
        tmp = tmp->next;
    }
    _figuraDx = head;

    tmp = _figuraSx;
    head = tmp;
    while (tmp != NULL)
    {
        tmp->x += inc_x;
        tmp->y += inc_y;
        tmp = tmp->next;
    }
    _figuraSx = head;
    return getFigura();
}

bool Weapon::getDirezione() {
    return _direzione;
}

bool Weapon::setDirezione(bool val){
    _direzione = val;
    return _direzione;
}
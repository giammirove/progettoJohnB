#include "oggetto.h"
#include "map.h"
#include "utility.h"
#include "bonus.h"

Bonus::Bonus(int x, int y, TipoDiOggetto tipo, ConvertiAsciiArt *asciiArt) : Oggetto(x, y, tipo, asciiArt) {
    impostaFigura(tipo, asciiArt);
}

/*
    Ritorna il bonus dato alla vita
*/
int Bonus::getBonusVita(){
    return _bonusVita;
}

/*
    Ritorna il bonus dato allo score
*/
int Bonus::getBonusScore(){
    return _bonusScore;
}

/*
    Ritorna il bonus dato all'arma
*/
int Bonus::getBonusArma(){
    return _bonusArma;
}

/*
    Ritorna il bonus bonus
*/
int Bonus::getBonusSalto(){
    return _bonusSalto;
}

/*
    Ritorna la probabilità di essere droppato
*/
int Bonus::getProbabilita(){
    return _probabilita;
}

/*
    Imposta la figura e i dati del bonus
*/
void Bonus::impostaFigura(TipoDiOggetto tipo, ConvertiAsciiArt *asciiArt)
{
	Oggetto::impostaFigura(tipo, asciiArt);
	switch (tipo)
	{

	case OS_BONUS_VITA:
        _bonusVita = 2;
        _bonusScore = 0;
        _bonusArma = 0;
        _bonusSalto = 0;
        _probabilita = getProbabilita(tipo);
		break;
    case OS_BONUS_ARMA:
        _bonusVita = 0;
        _bonusScore = 0;
        _bonusArma = 50;
        _bonusSalto = 0;
        _probabilita = getProbabilita(tipo);
        break;
    case OS_BONUS_SCORE:
        _bonusVita = 0;
        _bonusScore = 20;
        _bonusArma = 0;
        _bonusSalto = 0;
        _probabilita = getProbabilita(tipo);
        break; 
    case OS_BONUS_SALTO:
        _bonusVita = 0;
        _bonusScore = 0;
        _bonusArma = 0;
        _bonusSalto = 1;
        _probabilita = getProbabilita(tipo);
        break; 

	default:
        _bonusVita = 0;
        _bonusScore = 0;
        _bonusArma = 0;
        _bonusSalto = 0;
        _probabilita = getProbabilita(tipo);
		break;
	}
}

int Bonus::getProbabilita(TipoDiOggetto tipo) {
    int prob = 0;
    switch (tipo)
	{

	case OS_BONUS_VITA:
        prob = 30;
		break;
    case OS_BONUS_ARMA:
        prob = 10;
        break;
    case OS_BONUS_SCORE:
        prob = 40;
        break; 
    case OS_BONUS_SALTO:
        prob = 20;
        break; 

	default:
		break;
	}

    return prob;
}
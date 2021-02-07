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
		break;

	default:
        _bonusVita = 0;
        _bonusScore = 0;
        _bonusArma = 0;
		break;
	}
}
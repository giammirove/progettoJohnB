#include "oggetto.h"
#include "nemico.h"
#include "gestoreMovimento.h"
#include "map.h"

Nemico::Nemico(int x, int y, TipoDiOggetto tipo, ConvertiAsciiArt *asciiArt)
	: Oggetto(x, y, tipo, asciiArt)
{
	_direction = true;
	_MAX_INVULNERABILE = 0;	
	impostaFigura(tipo, asciiArt);
}

bool Nemico::minore_coppia(int x1, int y1, int x2, int y2)
{
	bool return_value = false; /* returna true se la prima coppia è minore della seconda */
	if (y1 == y2 && x1 < x2)
	{
		return_value = true;
	}
	else
	{
		if (y1 > y2)
			return_value = true;
	}

	return return_value;
}

bool Nemico::maggiore_coppia(int x1, int y1, int x2, int y2)
{
	bool return_value = false; /* returna true se la prima coppia è maggiore della seconda */
	if (y1 == y2 && x1 > x2)
	{
		return_value = true;
	}
	else
	{
		if (y1 > y2)
			return_value = true;
	}
	return return_value;
}

int *Nemico::getZampaDestra()
{
	figura buff_figura = getFigura();
	int maxX;
	int maxY;
	int *ret = new int(2);
	if (buff_figura != NULL)
	{
		maxX = buff_figura->x;
		maxY = buff_figura->y;
		while (buff_figura != NULL)
		{
			if (maggiore_coppia(buff_figura->x, buff_figura->y, maxX, maxY))
			{
				maxY = buff_figura->y;
				maxX = buff_figura->x;
			}
			buff_figura = buff_figura->next;
		}
	}
	ret[0] = maxX;
	ret[1] = maxY;

	return ret;
};

int *Nemico::getZampaSinistra()
{

	figura buff_figura = getFigura();
	int minX;
	int minY;
	int *ret = new int(2);
	if (buff_figura != NULL)
	{
		minX = buff_figura->x;
		minY = buff_figura->y;
		while (buff_figura != NULL)
		{
			if (minore_coppia(buff_figura->x, buff_figura->y, minX, minY))
			{
				minY = buff_figura->y;
				minX = buff_figura->x;
			}
			buff_figura = buff_figura->next;
		}
	}
	ret[0] = minX;
	ret[1] = minY;

	return ret;
}

/*
	Ritorna la vita del nemico
*/
int Nemico::getVita()
{
	return _vita;
}

/*
	Ritorna il danno del nemico
*/
int Nemico::getAttacco()
{
	return _attacco;
}

/*
	Ritorna lo score da lascia quando viene ucciso
*/
int Nemico::getScore(){
	return _score;
}

/*
	Ritorna se il nemico è statico
*/
bool Nemico::getStatico()
{
	return _statico;
}

/*
	Ritorna la posizione x per il drop del bonus
*/
int Nemico::getXBonus(){
	return getZampaSinistra()[0];
}

/*
	Ritorna la posizione y per il drop del bonus
*/
int Nemico::getYBonus(){
	return getZampaSinistra()[1];
}


/*
	True sse va a sinistra
	False sse va a destra
*/
bool Nemico::getDirection()
{
	return _direction;
}

bool Nemico::changeDirection()
{
	_direction = !_direction;
	return _direction;
}

/*
	Ritorna se il nemico è invulnerabile
*/
bool Nemico::getInvulnerabile()
{
	return _invulnerabile != 0;
}

/*
	Ritorna il numero di riferimento per l'invulnerabilità 
*/
int Nemico::getValoreInvulnerabile()
{
    return _invulnerabile;
}

/*
	Decrementa la variabile dell'incremento
*/
int Nemico::decrementaInvulnerabile(){
	_invulnerabile--;
	if(_invulnerabile < 0) _invulnerabile = 0;
	return _invulnerabile;
}

/*
	Cambia lo stato di invulnerabile in val
*/
bool Nemico::setInvulnerabile()
{
	_invulnerabile = _MAX_INVULNERABILE;
	return getInvulnerabile();
}

/*
	Riduce la vita del nemico di n
*/
int Nemico::decrementaVita(int n)
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
	Riduce la vita del nemico di 1
*/
int Nemico::decrementaVita()
{
	return decrementaVita(1);
}

void Nemico::vaiASinistra(int numeroPassi)
{
	figura buff_figura = getFigura();
	while (buff_figura != NULL)
	{
		buff_figura->x = buff_figura->x - numeroPassi;
		buff_figura = buff_figura->next;
	}
}

void Nemico::vaiADestra(int numeroPassi)
{
	figura buff_figura = getFigura();
	while (buff_figura != NULL)
	{
		buff_figura->x = buff_figura->x + numeroPassi;
		buff_figura = buff_figura->next;
	}
}

void Nemico::muoviNemico(Map *mappa)
{
	if (_statico == false)
	{
		if (getDirection())
		{ //true se sta andando a sinistra
			// sottrango l'offset in quanto il nemico è un oggetto in movimento
			if (mappa->controllaCollisione(getZampaSinistra()[0] - 1 - mappa->getOffset(), getZampaDestra()[1] + 1) != -1)
			{
				vaiASinistra(1);
			}
			else
			{

				changeDirection();
			}
		}
		else
		{
			// sottrango l'offset in quanto il nemico è un oggetto in movimento
			if (mappa->controllaCollisione(getZampaDestra()[0] + 1 - mappa->getOffset(), getZampaDestra()[1] + 1) != -1)
			{
				vaiADestra(1);
			}
			else
			{

				changeDirection();
			}
		}
	}
}

void Nemico::impostaFigura(TipoDiOggetto tipo, ConvertiAsciiArt *asciiArt)
{
	Oggetto::impostaFigura(tipo, asciiArt);
	switch (tipo)
	{

	case OS_NEMICO1:
		_vita = 1;
		_attacco = 1;
		_score = 5;
		_statico = false;
		break;

	case OS_NEMICO2:
		_vita = 2;
		_attacco = 2;
		_score = 10;
		_statico = false;
		break;

	case OS_PIATTAFORMA_APPUNTITA:
		_vita = 2;
		_attacco = 1;
		_statico = true;
		break;

	case OS_RAGNO:
		_vita = 3;
		_attacco = 2;
		_score = 15;
		_statico = false;
		break;

	default:
		break;
	}
}

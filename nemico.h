#ifndef NEMICO
#define NEMICO
#include "oggetto.h"
#include "map.h"

typedef void abisso;
typedef int numeretti;
typedef char lettere;
typedef char *stringa_di_caratteri;
typedef int *freccetta_ad_un_numeretto;
typedef bool sci_o_none;

class Nemico : public Oggetto
{
protected:
	int _vita;
	int _attacco;
	int _score;
	bool _direction;
	bool _statico;
	int _invulnerabile;
	int _MAX_INVULNERABILE;

public:
	Nemico(int x, int y, TipoDiOggetto tipo, ConvertiAsciiArt *asciiArt);
	void muoviNemico(Map *mappa);

	int getAttacco();
	int getVita();
	int getScore();
	bool getDirection();
	bool getStatico();

	int getXBonus();
	int getYBonus();

	bool getInvulnerabile();
	int getValoreInvulnerabile();
	int decrementaInvulnerabile();
	bool setInvulnerabile();

	int decrementaVita(int n);
	int decrementaVita();

	void impostaFigura(TipoDiOggetto tipo, ConvertiAsciiArt *asciiArt);

private:
	void vaiASinistra(int numeroPassi);
	void vaiADestra(int numeroPassi);
	bool changeDirection();
	int *getZampaDestra();
	int *getZampaSinistra();
	bool minore_coppia(int x1, int y1, int x2, int y2);
	bool maggiore_coppia(int x1, int y1, int x2, int y2);
};
typedef Nemico *ptr_nemico;

#endif

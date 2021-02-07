#ifndef BONUS
#define BONUS

#include "oggetto.h"
#include "map.h"
#include "utility.h"

class Bonus : public Oggetto {

    private :
        int _bonusVita;
        int _bonusScore;
        int _bonusArma;

    public:
        Bonus(int x, int y, TipoDiOggetto tipo, ConvertiAsciiArt *asciiArt);

        int getBonusVita();
        int getBonusScore();
        int getBonusArma();

        void impostaFigura(TipoDiOggetto tipo, ConvertiAsciiArt *asciiArt);
};

#endif
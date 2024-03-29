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
        int _bonusSalto;
        int _probabilita;

    public:
        Bonus(int x, int y, TipoDiOggetto tipo, ConvertiAsciiArt *asciiArt);

        int getBonusVita();
        int getBonusScore();
        int getBonusArma();
        int getBonusSalto();
        int getProbabilita();

        static int getProbabilita(TipoDiOggetto tipo);

        void impostaFigura(TipoDiOggetto tipo, ConvertiAsciiArt *asciiArt);
};

#endif
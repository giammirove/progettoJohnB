#ifndef CONVERTIASCIIART_H_INCLUDED
#define CONVERTIASCIIART_H_INCLUDED

#include <string.h>
#include <iostream>
#include <fstream>
#include "utility.h"

using namespace std;

class ConvertiAsciiArt
{
private:
    lista_figura _lista_figura;
    void initFigure(FILE *read);
    void aggiungiFigura(figura fig, const char nome[]);

public:
    ConvertiAsciiArt(FILE *read);
    lista_figura getListaFigura();
    figura getFigura(const char nome[]);
    bool esisteFigura(const char nome[]);
};

void test();

#endif
#include <string.h>
#include <fstream>
#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include "convertiAsciiArt.h"
#include "utility.h"

using namespace std;

/*
    Costrutture della classe ConvertiAsciiArt
    read = flusso di dati che permette di leggere il file con le figure da caricare (es. "asciiArtDB.txt")
*/
ConvertiAsciiArt::ConvertiAsciiArt(FILE *read)
{
    initFigure(read);
}

/*
    Ritorna la lista di figure caricate dal file scelto con il costruttore
*/
lista_figura ConvertiAsciiArt::getListaFigura()
{
    return _lista_figura;
}

/*
    Ritorna una figura presente in lista associata a nome
    nome = indica il nome della figura da ritornare
*/
figura ConvertiAsciiArt::getFigura(const char nome[])
{
    lista_figura t = _lista_figura;
    bool found = false;
    figura f = NULL;
    while (t != NULL && found == false)
    {
        if (strcmp(nome, t->nome) == 0)
        {
            found = true;
            f = t->_figura;
        }
        else
            t = t->next;
    }

    return f;
}

/*
    Verifica se la figura è presente in lista
    DA UTILIZZARE PRIMA DI RICHIAMARE LA FUNZIONE getFigura
    nome = indica il nome dalla figura per la quale va verificata la presenza
*/
bool ConvertiAsciiArt::esisteFigura(const char nome[])
{
    lista_figura t = _lista_figura;
    bool found = false;
    while (t != NULL && found == false)
    {
        removeLastSpace(t->nome);
        if (strcmp(nome, t->nome) == 0)
        {
            found = true;
        }
        else
            t = t->next;
    }

    return found;
}

// PRIVATE

/*
    Legge il file preso dallo stream read
    Esempio di struttura
    ::PG
     @
    /|\
    / \
    ::
*/
void ConvertiAsciiArt::initFigure(FILE *read)
{
    char line[256];
    char nomeFig[30];
    figura fig;

    int n_line = 0;
    while (fgets(line, 256, read))
    {
        removeChar(line, '\n');
        removeChar(line, '\t');
        removeChar(line, '\r');
        int len = strlen(line);
        if (len >= 2)
        {
            if (line[0] == ':' && line[1] == ':')
            {
                // eslude i primi 2 cioè ::
                if (fig != NULL)
                {
                    // rimuove i caratteri particolari generati dalla codifica
                    removeChar(nomeFig, '\n');
                    removeChar(nomeFig, '\t');
                    removeChar(nomeFig, '\r');
                    removeLastSpace(nomeFig);
                    aggiungiFigura(fig, nomeFig);
                }
                // line + 2 sposta di spazi la linea letta, ovvero rimuove ::
                strcpy(nomeFig, line + 2);
                fig = new figura_t;
                n_line = 0;
                continue;
            }
        }

        for (int i = 0; i < len; i++)
        {
            if (line[i] != ' ')
            {
                char c[2] = {line[i], '\0'};
                aggiungiPuntoAFigura(&fig, i, n_line, c);
            }
        }

        n_line++;
    }
}

/*
    Aggiunge la figura alla lista 
    fig = indica l'oggetto figura da inserire in lista
    nome = indica il nome al quale associare fig in lista
*/
void ConvertiAsciiArt::aggiungiFigura(figura fig, const char nome[])
{
    if (_lista_figura == NULL)
    {
        _lista_figura = new lista_figura_t;
        _lista_figura->_figura = fig;
        strcpy(_lista_figura->nome, nome);
        _lista_figura->next = NULL;
    }
    else
    {
        lista_figura t = new lista_figura_t;
        t->_figura = fig;
        strcpy(t->nome, nome);
        t->next = _lista_figura;
        _lista_figura = t;
    }
}
#include <string.h>
#include "utility.h"

/*
    Ritorna una stringa invertita rispetto all'input
*/
void reverse(char str[])
{
    int len = strlen(str);

    for (int i = 0; i < (int)(len / 2); i++)
    {
        char t = str[i];
        str[i] = str[(len - 1) - i];
        str[(len - 1) - i] = t;
    }
}

// convert int to char
char *itoaMine(int n)
{
    /*
        n = 23
        134 % 10 = 4
        134 - 4 = 130
        130 / 10 = 13

        13 % 10 = 3
        13 - 3 = 10
        10 / 10 = 1

        1 % 10 = 1
        1 - 1 = 0
    */

    int r = 0;
    int i = 0;
    char *str = new char(256);
    if (n == 0)
        str[0] = '0';
    else
    {
        while (n > 0)
        {
            r = n % 10;
            n = n - r;
            n = n / 10;
            str[i] = r + '0';
            i++;
        }
        reverse(str);
    }

    str[i + 1] = '\0';
    return str;
}

// convert string to int
int atoiMine(const char str[])
{
    int len = strlen(str);
    int i = len - 1;
    int dec = 1;
    int n = 0;
    while (i >= 0)
    {
        n = n + (str[i] - 48) * dec;
        dec = dec * 10;
        i--;
    }

    return n;
}

void shift(char str[], int n)
{
    // ciao come
    // come
    // str[4] =

    int i = n;
    int len = strlen(str);
    while (i < len)
    {
        str[i - n] = str[i];
        i++;
    }
    str[len - n] = '\0';
}

void strtokMine(char line[], char out[], char fl)
{
    int len = strlen(line);
    if (len > 0)
    {
        int foundFl = 0;
        int i = 0, o = 0;
        while (i < len && foundFl == 0)
        {
            if (line[i] == fl)
            {
                foundFl = 1;
            }
            else
            {
                out[i] = line[i];
                i++;
            }
        }

        if (foundFl == 0)
        {
            line[0] = '\0';
        }
        else
        {
            shift(line, i + 1);
        }

        // termino la stringa
        out[i] = '\0';
    }
    else
    {
        line = NULL;
        out = NULL;
    }
}

/*
    Rimuove un particolare carattere (c) da una stringa (s)
*/
void removeChar(char s[], int c)
{

    int j = 0, n = strlen(s);
    for (int i = 0; i < n; i++)
        if (s[i] != c)
        {
            s[j] = s[i];
            j++;
        }

    s[j] = '\0';
}

/*
    Rimuove i commenti da una stringa (s) , ovvero tutto ciò che viene dopo //
*/
void removeComment(char s[])
{
    int i = 0, n = strlen(s), found = 0;
    while(i < n && found == 0)
    {
        if (s[i] == '/' && i+1<n)
        {
            if(s[i+1] == '/')
            {
                found = 1;
            }
            else
            {
                i++;
            }
        }
        else
        {
            i++;
        }
    }

    s[i] = '\0';
}

/*
    Rimuove gli spazi vuoti iniziali
*/ 
void removeInitialSpace(char s[])
{
    int i = 0, n = strlen(s), found = 0;
    while(i < n && found == 0)
    {
        if (s[i] != ' ')
        {
            found = 1;
        }
        else
        {
            i++;
        }
    }

    shift(s, i);
}

/*
    Rimuove gli spazi vuoti finali
*/
void removeLastSpace(char s[])
{
    int i = 0, n = strlen(s), found = 0;
    while(i < n && found == 0)
    {
        if (s[i] == ' ')
        {
            s[i] = '\0';
            found = 1;
        }
        else
        {
            i++;
        }
    }
}

/*
    Aggiunge un punto (c) alle coordinate x,y alla figura (fig) 
*/
void aggiungiPuntoAFigura(figura *_figura, int x, int y, const char c[])
{
    if (_figura == NULL)
    {
        *_figura = new figura_t;
        (*_figura)->x = x;
        (*_figura)->y = y;
        strcpy((*_figura)->c, c);
        (*_figura)->next = NULL;
    }
    else
    {
        figura t = new figura_t;
        t->x = x;
        t->y = y;
        strcpy(t->c, c);
        t->next = (*_figura);
        (*_figura) = t;
    }
}


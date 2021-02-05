#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

// genera un numero intero casuale
int randomNumber (int min, int max);

// reverse a string
void reverse(char str[]);

// convert int to char
char *itoaMine(int n);

// convert char to int
int atoiMine(const char str[]);

void shift(char str[], int n);

void strtokMine(char line[], char out[], char fl);

void removeChar(char s[], int c);

void removeComment(char s[]);

void removeInitialSpace(char s[]);

void removeLastSpace(char s[]);

struct figura_t
{
    int x;
    int y;
    char c[30];
    figura_t *next;
};
typedef struct figura_t *figura;

struct lista_figura_t
{
    char nome[30];
    figura _figura;
    lista_figura_t *next;
};
typedef struct lista_figura_t *lista_figura;

void aggiungiPuntoAFigura(figura *_figura, int x, int y, const char c[]);

#endif
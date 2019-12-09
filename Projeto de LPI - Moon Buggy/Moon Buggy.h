#ifndef MOON_BUGGY_H_INCLUDED
#define MOON_BUGGY_H_INCLUDED


typedef struct _janela
{
    short linhas;
    short colunas;
}janela;

typedef struct _coordenadas
{
    short x;
    short y;
}coordenadas;

typedef struct _elementos
{
    coordenadas estrada;
    coordenadas buggy;
    coordenadas HUD;
    coordenadas info;
}elementos;

void init_game(janela J);


#endif // MOON_BUGGY_H_INCLUDED

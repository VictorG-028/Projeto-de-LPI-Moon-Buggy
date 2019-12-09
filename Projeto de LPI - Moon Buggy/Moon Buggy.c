#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <stdbool.h>
#include <windows.h>

#include "Moon Buggy.h"

/**
 * Observação: Eixo X e Y invertidos
 * Eixo X é vertical, ou seja representa as linhas (altura)
 * Eixo Y é horizontal, ou seja representa as colunas (distância da borda esquerda)
 *
 *   x
 *  ^
 *  |  .(y, x)
 *  |
 *  L----->
 *         y
 */

void gotoxy(short x, short y)
{
    COORD position = {y, x}; // INVERSÃO DOS EIXOS
    SetConsoleCursorPosition( GetStdHandle(STD_OUTPUT_HANDLE), position);
}


void desenha_cenario(janela J, elementos E, short level, short vida, short pontos)
{
    int contador = 0;

    // Desenha a estrada
    gotoxy(E.estrada.x, E.estrada.y);

    do
    {
        printf("#");
        contador++;

    } while (contador < J.colunas);

    gotoxy(E.estrada.x +1, E.estrada.y);

    contador = 0;
    do
    {
        printf("#");
        contador++;

    } while (contador < J.colunas);


    // Desenha o buggy
    gotoxy(E.buggy.x, E.buggy.y);
    printf("   Omn ");

    gotoxy(E.buggy.x +1, E.buggy.y);
    printf("(\\)-(\\)");


    // Desenha HUD (level, life, score, controles)
    gotoxy(E.HUD.x, E.HUD.y);
    printf("Level: %d    Life: %d    Score: %d", level, vida, pontos);
}


void pause(elementos E)
{
    // variáveis
    char *continuar_msg = "Continuar";
    char *salvar_msg = "Salvar e sair";
    char input = -1;

    bool pausa = true;
    bool novo_frame = false;

    int i = 0;

    coordenadas seta = {E.info.x, E.info.y -strlen(continuar_msg)/2 -4};
    coordenadas continuar = {E.info.x, E.info.y -strlen(continuar_msg)/2};
    coordenadas salvar = {E.info.x +2, E.info.y -strlen(salvar_msg)/2};


    // Desenha as opções
    gotoxy(continuar.x, continuar.y);
    printf("%s", continuar_msg);

    gotoxy(salvar.x, salvar.y);
    printf("%s", salvar_msg);

    // Desenha a seta
    gotoxy(seta.x, seta.y);
    printf("-->");

    while (pausa)
    {
        // Pega input do usuário caso uma tecla seja precionada
        if (kbhit()) input = getch();

        // Processa o input
        switch (input)
        {
        case 'w':
            if (seta.x -2 >= E.info.x)
            {
                // Apaga a seta atual
                gotoxy(seta.x, seta.y);
                printf("   ");

                seta.x += -2; // Movimenta a seta para baixo
                seta.y = continuar.y -4; // Movimenta a seta para trás da palavra

                novo_frame = true; // Re-desenha a seta
            }
            break;

        case 's':
            if(seta.x +2 <= E.info.x +2)
            {
                // Apaga a seta atual
                gotoxy(seta.x, seta.y);
                printf("   ");

                seta.x += +2; // Movimenta a seta para cima
                seta.y = salvar.y -4; // Movimenta a seta para trás da palavra

                novo_frame = true; // Re-desenha a seta
            }
            break;

        case ' ':
            if(seta.x == E.info.x)
            {
                pausa = false;
            }
            else
            {
                pausa = false;
                // Salvar rank e sair do jogo
                // LEMBRAR DE ARRUMAR AKI <-------------------
            }
            break;

        } // Fim d switch (input)

        // Renderiza as mudanças
        if (novo_frame)
        {
            // Re-desenha a seta
            gotoxy(seta.x, seta.y);
            printf("-->");

            novo_frame = false;
        }

        // Limpa o input
        input = -1;

    } // Fim do while (pause)

    // Apaga as mensagens do menu da pausa
    gotoxy(seta.x, seta.y);
    printf("   ");

    gotoxy(1, 1);
    printf("       ");

    gotoxy(continuar.x, continuar.y);
    do { printf(" "); i++; } while(i < strlen(continuar_msg) );

    i = 0; // Reseta o contador

    gotoxy(salvar.x, salvar.y);
    do { printf(" "); i++; } while(i < strlen(salvar_msg) );

} // Fim da função pause()


void game_loop(elementos E, short level,short *vida, short *pontos)
{
    // Variáveis
    short frame_roda = 0;
    short frame_chassi = 0;
    short frame_cabine = 0;

    char roda[] = "\\-/|U";
    char chassi[] = "mmMM";
    char cabine[] = "OoOo";

    char input; // Contém o input do usuário

    bool pular = false, subir = false, descer = false;

    bool game_loop = true;

    while (game_loop)
    {
        // Pega input do usuário caso uma tecla seja precionada
        Sleep(150);
        if (kbhit()) input = getch();

        // Processa o input
        switch (input)
        {
        case ' ':
            if(pular == false)
            {
                pular = true;
                subir = true;
            }
            break;
        case 'p':
            gotoxy(1, 1);
            printf("Pausado");
            pause(E);
            break;

        } // Fim do switch input

        // Renderiza as mudanças

        if (pular)
        {
            // Atualiza a altura
            if (subir)
            {
                E.buggy.x += -1; // Sobe o buggy

                if (E.buggy.x < E.estrada.x -5)
                {
                    subir = false;
                    descer = true;
                }
            }
            else if (descer)
            {
                E.buggy.x += +1; // Desce o buggy

                if (E.buggy.x > E.estrada.x -3)
                {
                    descer = false;
                    pular = false; // Fim da animação do pulo

                    frame_chassi = 0;
                    frame_cabine = 0;
                }
            }

            // Desenha o carro pulando
            gotoxy(E.buggy.x, E.buggy.y);
            printf("   %c%cn ", cabine[frame_cabine], chassi[frame_chassi]);

            gotoxy(E.buggy.x +1, E.buggy.y);
            if(E.buggy.x == E.estrada.x -2 || E.buggy.x == E.estrada.x -2)
            {
                printf("(U)-(U)");
            }
            else
            {
                printf("(%c)-(%c)", roda[frame_roda], roda[frame_roda]);
            }

            gotoxy(E.buggy.x +2, E.buggy.y); // Antiga posição das rodas
            if (subir || E.buggy.x != E.estrada.x -2) printf("       "); // Remove as rodas da tela

            gotoxy(E.buggy.x -1, E.buggy.y); // Antiga posição do chassi
            if (descer || E.buggy.x != E.estrada.x -3) printf("       "); // Remove o chassi do buggy da tela

            // Atualiza o frame do chassi e cabine
            if(frame_chassi < 3)
            {
                frame_chassi++;
                frame_cabine++;
            }
            else
            {
                frame_chassi = 0;
                frame_cabine = 0;
            }
        }
        else
        {
            // Animação da roda do carro
            gotoxy(E.buggy.x +1, E.buggy.y +1);
            printf("%c", roda[frame_roda]);

            gotoxy(E.buggy.x +1, E.buggy.y +5);
            printf("%c", roda[frame_roda]);

            // Atualiza o frame da roda
            if(frame_roda < 3)
            {
                frame_roda++;
            }
            else
            {
                frame_roda = 0;
            }
        }

        // Limpa o input
        input = -1;

        // Atualiza o pontos
        *pontos += 1;

        // Atualiza o HUD
        gotoxy(E.HUD.x, E.HUD.y);
        printf("Level: %d    Life: %d    Score: %d", level, *vida, *pontos);

    } // Fim do while (game_loop)

} // Fim da função game_loop()


void level_1(janela J, elementos E, short *vida, short *pontos)
{
    //Variáveis
    char input;
    char *msg_1 = "Pule os buracos para passar de fase ";
    char *msg_2 = "Pressione ESPACO para iniciar ";

    short i = 0;

    desenha_cenario(J, E, 1, *vida, *pontos);

    // Desenha informações adicionais do level 1
    gotoxy(E.info.x, E.info.y -strlen(msg_1)/2);
    printf("%s", msg_1);

    gotoxy(E.info.x +1, E.info.y -strlen(msg_2)/2);
    printf("%s", msg_2);

    do { input = getch(); } while(input != ' ');


    // Apaga a mensagem inicial
    gotoxy(E.info.x, E.info.y -strlen(msg_1)/2);
    do { printf(" "); i++; } while (i < strlen(msg_1) );

    i = 0; // Reseta o contador

    gotoxy(E.info.x +1, E.info.y -strlen(msg_2)/2);
    do { printf(" "); i++; } while(i < strlen(msg_2) );

    game_loop(E, 1, vida, pontos); // Inicia a gameplay

} // Fim da função level_1()


void level_2(janela J, elementos E, short *vida, short *pontos)
{
    // ...
}


void level_3(janela J, elementos E, short *vida, short *pontos)
{
    // ...
}


void play_game(janela J)
{
    short vida = 3;
    short pontos = 0;

    elementos E; // Guarda as coordenadas dos elementos da janela

    E.estrada.x = J.linhas*3/4;
    E.estrada.y = 0;

    E.buggy.x = J.linhas*3/4 -2;
    E.buggy.y = J.colunas*3/4;

    E.HUD.x = J.linhas*3/4 +2;
    E.HUD.y = J.colunas/2 -15;

    E.info.x = J.linhas*3/4 -6;
    E.info.y = J.colunas/2;

    level_1(J, E, &vida, &pontos);

    level_2(J, E, &vida, &pontos);

    level_3(J, E, &vida, &pontos);
}


void play_game_old(janela J)
{
    // Variáveis
    short contador = 0;

    int frame_roda = 0;
    int frame_chassi = 0;
    int frame_cabine = 0;

    int level = 1;
    short vida = 3;
    short pontos = 0;

    char roda[] = "\\-/|U";
    char chassi[] = "mmMM";
    char cabine[] = "OoOo";
    char input; // Contém o input do usuário

    bool pular = false, subir = false, descer = false;
    bool game_loop = true;

    coordenadas estrada = {J.linhas*3/4, 0};
    coordenadas buggy = {J.linhas*3/4 -2, J.colunas*3/4};
    coordenadas HUD = {estrada.x +2, J.colunas/2 -15};

    // Desenha a estrada
    gotoxy(estrada.x, estrada.y);

    do
    {
        printf("#");
        contador++;

    } while (contador < J.colunas);

    gotoxy(estrada.x +1, estrada.y);

    contador = 0;
    do
    {
        printf("#");
        contador++;

    } while (contador < J.colunas);


    // Desenha o buggy
    gotoxy(buggy.x, buggy.y);
    printf("   Omn ");

    gotoxy(buggy.x +1, buggy.y);
    printf("(\\)-(\\)");


    // Desenha HUD (level, life, score, controles)
    gotoxy(HUD.x, HUD.y);
    printf("Level: %d    Life: %d    Score %d", level, vida, pontos);


    while (game_loop)
    {
        // Pega input do usuário caso uma tecla seja precionada
        Sleep(150);
        if (kbhit()) input = getch();

        // Processa o input
        switch (input)
        {
        case ' ':
            if(pular == false)
            {
                pular = true;
                subir = true;
            }
            break;
        case 'p':
            //pause();
            gotoxy(1, 1);
            printf("Pause");
            break;

        } // Fim do switch input

        // Renderiza as mudanças
        if (pular)
        {
            // Atualiza a altura
            if (subir)
            {
                buggy.x += -1; // Sobe o buggy

                if (buggy.x < estrada.x -5)
                {
                    subir = false;
                    descer = true;
                }
            }
            else if (descer)
            {
                buggy.x += +1; // Desce o buggy

                if (buggy.x > estrada.x -3)
                {
                    descer = false;
                    pular = false; // Fim da animação do pulo

                    frame_chassi = 0;
                    frame_cabine = 0;
                }
            }

            // Desenha o carro pulando
            gotoxy(buggy.x, buggy.y);
            printf("   %c%cn ", cabine[frame_cabine], chassi[frame_chassi]);

            gotoxy(buggy.x +1, buggy.y);
            if(buggy.x == estrada.x -2 || buggy.x == estrada.x -2)
            {
                printf("(U)-(U)");
            }
            else
            {
                printf("(%c)-(%c)", roda[frame_roda], roda[frame_roda]);
            }

            gotoxy(buggy.x +2, buggy.y); // Antiga posição das rodas
            if (subir || buggy.x != estrada.x -2) printf("       "); // Remove as rodas da tela

            gotoxy(buggy.x -1, buggy.y); // Antiga posição do chassi
            if (descer || buggy.x != estrada.x -3) printf("       "); // Remove o chassi do buggy da tela

            // Atualiza o frame do chassi e cabine
            if(frame_chassi < 3)
            {
                frame_chassi++;
                frame_cabine++;
            }
            else
            {
                frame_chassi = 0;
                frame_cabine = 0;
            }
        }
        else
        {
            // Animação da roda do carro
            gotoxy(buggy.x +1, buggy.y +1);
            printf("%c", roda[frame_roda]);

            gotoxy(buggy.x +1, buggy.y +5);
            printf("%c", roda[frame_roda]);

            // Atualiza o frame da roda
            if(frame_roda < 3)
            {
                frame_roda++;
            }
            else
            {
                frame_roda = 0;
            }
        }

        // Limpa o input
        input = -1;

    } // Fim do while (game_loop)

} // Fim da função play_game()


void menu(janela J)
{
    // Variáveis
    char input = -1; // Contém o input do usuário

    bool novo_frame = true;
    bool menu = true;

    coordenadas info = {J.linhas/2, J.colunas/8};
    coordenadas seta = {J.linhas/2, J.colunas/2 -6};
    coordenadas play = {J.linhas/2, J.colunas/2 -2};
    coordenadas rank = {J.linhas/2 +2, J.colunas/2 -2};
    coordenadas exit = {J.linhas/2 +4, J.colunas/2 -2};


    // Desenha o título do jogo
    gotoxy(J.linhas/4, J.colunas/2 -5);
    printf("Moon Buggy");


    //Desenha os controles/informações do jogo
    gotoxy(info.x, info.y);
    printf("Mover seta: W(cima) S(baixo)");

    gotoxy(info.x +1, info.y);
    printf("Selecionar: espaco(escolher)");


    // Desenha a seta de seleção -->
    gotoxy(seta.x, seta.y);
    printf("-->");


    // Desenha o botão PLAY
    gotoxy(play.x, play.y);
    printf("PLAY");


    //Desenha o botão RANK
    gotoxy(rank.x, rank.y);
    printf("RANK");


    //Desenha o botão EXIT
    gotoxy(exit.x, exit.y);
    printf("EXIT");


    while (menu)
    {
        // Pega input do usuário caso uma tecla seja precionada
        if (kbhit()) input = getch();

        // Processa o input
        switch (input)
        {
        case 'w':
            if (seta.x -2 >= J.linhas/2)
            {
                // Apaga a seta atual
                gotoxy(seta.x, seta.y);
                printf("   ");

                seta.x += -2; // Movimenta a seta

                novo_frame = true; // Re-desenha a seta para cima
            }
            break;

        case 's':
            if (seta.x +2 <= J.linhas/2 +4)
            {
                // Apaga a seta atual
                gotoxy(seta.x, seta.y);
                printf("   ");

                seta.x += 2; // Movimenta a seta

                novo_frame = true; // Re-desenha a seta para baixo
            }
            break;

        case ' ': // espaço = enter

            if (seta.x == play.x)
            {
                system("cls");
                play_game(J);
            }
            else if (seta.x == rank.x)
            {
                system("cls");
                //show_rank(J);
            }
            else if (seta.x == seta.x)
            {
                menu = false;
            }
            break;


        } // Fim do switch (input)

        // Renderiza as mudanças
        if (novo_frame)
        {
            // Re-desenha a seta
            gotoxy(seta.x, seta.y);
            printf("-->");

            novo_frame = false;
        }

        // Limpa o input
        input = -1;

    } // Fim do while (menu)

} // Fim da função menu()


void init_game(janela J)
{
    menu(J);
}

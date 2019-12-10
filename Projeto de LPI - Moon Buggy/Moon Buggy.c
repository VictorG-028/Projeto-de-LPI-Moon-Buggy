#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <stdbool.h>
#include <windows.h>

#include "Moon Buggy.h"

/**
 * Observa��o: Eixo X e Y invertidos
 * Eixo X � VERTICAL, ou seja representa as linhas (altura)
 * Eixo Y � HORIZONTAL, ou seja representa as colunas (dist�ncia da borda esquerda)
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
    COORD position = {y, x}; // INVERS�O DOS EIXOS
    SetConsoleCursorPosition( GetStdHandle(STD_OUTPUT_HANDLE), position);
}


void show_rank(janela J)
{
    // vari�vel
    char *texto;

    // Coloca o primeiro texto
    texto = "Aperte qualquer coisa para voltar ao menu";
    gotoxy(J.linhas/2, J.colunas/2 - strlen(texto)/2);
    puts(texto);

    // Coloca o segundo texto
    texto = "Sistema de rank ainda nao implementado";
    gotoxy(J.linhas/2 +1, J.colunas/2 - strlen(texto)/2);
    puts(texto);

    getch();
    system("cls");
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
    // vari�veis
    char *continuar_msg = "Continuar";
    char *salvar_msg = "Salvar e sair";
    char input = -1;

    bool pausa = true;
    bool novo_frame = false;

    int i = 0;

    coordenadas seta = {E.info.x, E.info.y -strlen(continuar_msg)/2 -4};
    coordenadas continuar = {E.info.x, E.info.y -strlen(continuar_msg)/2};
    coordenadas salvar = {E.info.x +2, E.info.y -strlen(salvar_msg)/2};


    // Desenha as op��es
    gotoxy(continuar.x, continuar.y);
    printf("%s", continuar_msg);

    gotoxy(salvar.x, salvar.y);
    printf("%s", salvar_msg);

    // Desenha a seta
    gotoxy(seta.x, seta.y);
    printf("-->");

    while (pausa)
    {
        // Pega input do usu�rio caso uma tecla seja precionada
        if ( kbhit() ) input = getch();

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
                seta.y = continuar.y -4; // Movimenta a seta para tr�s da palavra

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
                seta.y = salvar.y -4; // Movimenta a seta para tr�s da palavra

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

        // Renderiza as mudan�as
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

} // Fim da fun��o pause()


void msg_do_level(elementos E, short level_atual)
{
    // Vari�veis
    char input;
    char *msg_1;
    char *msg_2;

    short i = 0;

    coordenadas _msg_1;
    coordenadas _msg_2;

    if (level_atual == 1)
    {
        msg_1 = "Pule <SPC> os buracos para passar de fase ";
        msg_2 = "Pressione ESPACO para iniciar ";
    }
    else if (level_atual == 2)
    {
        msg_1 = "Buracos maiores a frente!!!";
        msg_2 = "Pressiona ESPACO para continuar";
    }
    else if (level_atual == 3)
    {
        msg_1 = "Pilhas de pedra e areia na frente!!!";
        msg_2 = "Atire <Z> para continuar!";
    }
    else
    {
        msg_1 = "ERRO";
        msg_2 = "ERRO";
    }

    // Calcula as coodenadas com base na msg que vai ser escrita
    _msg_1.x = E.info.x;
    _msg_1.y = E.info.y -strlen(msg_1)/2;

    _msg_2.x = E.info.x +1;
    _msg_2.y = E.info.y -strlen(msg_2)/2;

    // Desenha informa��es adicionais do level 1
    gotoxy(_msg_1.x, _msg_1.y);
    printf("%s", msg_1);

    gotoxy(_msg_2.x, _msg_2.y);
    printf("%s", msg_2);

    // Espera o input
    if(level_atual != 3)
    {
        do { input = getch(); } while(input != ' ');
    }
    else
    {
        do { input = getch(); } while(input != 'z');
    }

    // Apaga as mensagens
    gotoxy(_msg_1.x, _msg_1.y);
    do { printf(" "); i++; } while (i < strlen(msg_1) );

    i = 0; // Reseta o contador

    gotoxy(_msg_2.x, _msg_2.y);
    do { printf(" "); i++; } while(i < strlen(msg_2) );

} // Fim da fun��o msg_do_level();


void logica_pular(elementos *E, bool *pular, bool *subir, bool *descer)
{
    // Atualiza a altura
    if (*subir)
    {
        E->buggy.x += -1; // Sobe o buggy

        if (E->buggy.x < E->estrada.x -5) // Limita a altura max de subida
        {
            // Inverte subir e descer
            *subir = false;
            *descer = true;
        }
    }
    else if (*descer)
    {
        E->buggy.x += +1; // Desce o buggy

        if (E->buggy.x > E->estrada.x -3) // Limita a descida at� o ch�o
        {
            // Finaliza��o da anima��o do pulo
            *descer = false;
            *pular = false;
        }
    }

} // Fim da fun��o pular()


void desenhar_pulo(elementos E,bool subir, short *frame_cabine, short *frame_chassi, short *frame_roda)
{
    // Vari�veis
    char roda[] = "\\-/|U";
    char chassi[] = "mmMM";
    char cabine[] = "OoOo";

    // Desenha o carro pulando
    gotoxy(E.buggy.x, E.buggy.y);
    printf("   %c%cn ", cabine[*frame_cabine], chassi[*frame_chassi]);

    gotoxy(E.buggy.x +1, E.buggy.y);
    if(E.buggy.x == E.estrada.x -2 || E.buggy.x == E.estrada.x -2)
    {
        printf("(U)-(U)");
    }
    else
    {
        printf("(%c)-(%c)", roda[*frame_roda], roda[*frame_roda]);
    }

    gotoxy(E.buggy.x +2, E.buggy.y); // Antiga posi��o das rodas
    if (subir || E.buggy.x != E.estrada.x -2) printf("       "); // Remove as rodas da tela

    gotoxy(E.buggy.x -1, E.buggy.y); // Antiga posi��o do chassi
    if (!subir || E.buggy.x != E.estrada.x -3) printf("       "); // Remove o chassi do buggy da tela

    // Atualiza o frame do chassi e cabine
    if(*frame_chassi < 3)
    {
        *frame_chassi += 1;
        *frame_cabine += 1;
    }
    else
    {
        *frame_chassi = 0;
        *frame_cabine = 0;
    }

} // Fim da fun��o desenhar_pulo()


void gerar_obstaculo(short *obstaculos, short level, short max, short *espacamento)
{
    // Vari�veis
    // short tamanho;
    short i;

    if (level < 3) // cria um buraco
    {
        // So gera um obst�culo ap�s espa�amento chegar a 0 novamente
        if (*espacamento == 0)
        {
            gotoxy(1, 1);
            printf("AKI 2");

            for(i = 0; i < max; i++)
            {
                if (obstaculos[i] == -1)
                {
                    obstaculos[i] = 0;
                    *espacamento = rand() % 10 +11; // Gera n�meros de 10 � 20
                    break;
                }
            }
        }
    }
    else // Sen�o cria um monte de pedra e areia
    {

    }
}


void game_loop(elementos E, short *level,short *vida, short *pontos)
{
    // Vari�veis
    short frame_roda = 0;
    short frame_chassi = 0;
    short frame_cabine = 0;

    short i = 0;
    short max = 10; // M�x de 10 obst�culos no jogo
    short obstaculos[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    short espacamento = 0; // Espa�o de estrada entre obst�culos
    short hitbox = E.buggy.y;
    short limite_direita = E.buggy.y*4/3;

    char roda[] = "\\-/|U"; // Charact�res da roda

    char input; // Cont�m o input do usu�rio

    bool pular = false, subir = false, descer = false;

    bool game_loop = true;

    while (game_loop)
    {
        // Pega input do usu�rio caso uma tecla seja precionada
        Sleep(150);
        if ( kbhit() ) input = getch();

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
        case 'z':
            if(*level >= 3)
            {

                //atirar(); // L�gica do tiro
            }
            break;
        case 'p':
            gotoxy(1, 1);
            printf("Pausado");
            pause(E);
            break;

        } // Fim do switch input

        // Renderiza as mudan�as
        if (pular)
        {
            // L�gica do pulo
            logica_pular(&E, &pular, &subir, &descer);

            // Desenha o carro enquanto pula
            desenhar_pulo(E, subir, &frame_chassi, &frame_cabine, &frame_roda);
        }
        else // Sen�o est� pulando, est� rodando a roda
        {
            // Ajusta o frame da cabine e do chassi ap�s o pulo
            gotoxy(E.buggy.x, E.buggy.y);
            printf("   Omn");

            // Anima��o da roda do carro
            gotoxy(E.buggy.x +1, E.buggy.y +1);
            printf("%c", roda[frame_roda]);

            gotoxy(E.buggy.x +1, E.buggy.y +5);
            printf("%c", roda[frame_roda]);

            // Atualiza o frame da roda
            if (frame_roda < 3)
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

        // Gera os obst�culos de acordo com o level
        if (*level == 1 && *pontos > 10)
        {
            gotoxy(1, 1); printf("Gerou obstaculo");
            gerar_obstaculo(obstaculos, *level, max, &espacamento);
        }
        else if (*level == 2 && *pontos > 210)
        {
            gerar_obstaculo(obstaculos, *level, max, &espacamento);
        }
        else if(*level == 3 && *pontos > 410)
        {
            gerar_obstaculo(obstaculos, *level, max, &espacamento);
        }

        // Atualiza os obst�culosa
        for(i = 0; i < max; i++)
        {
            // Atualiza a posi��o se o obst�culo existir
            if (obstaculos[i] != -1) obstaculos[i] += 1;

            // Retira o obst�culo ao chegar no fim da estrada
            if (obstaculos[i] == limite_direita) obstaculos[i] = -1;
        }

        // Desenha os buracos
        for(i = 0; i < max; i++)
        {
            // Desenha apenas os obst�culos existentes
            if (obstaculos[i] != -1)
            {
                gotoxy(E.estrada.x, obstaculos[i]);
                printf(" ");

                gotoxy(E.estrada.x, obstaculos[i] -1);
                printf("#");
            }
        }

        // Confere se o carro n�o caio ou bateu
        for(i = 0; i < max; i++)
        {
            if (hitbox == obstaculos[i] && E.buggy.x == E.estrada.x -2)
            {
                *vida += -1; // Perde 1 vida

                // Mensagem de RIP
                gotoxy(E.info.x, E.info.y -2);
                printf("RIP ");
            }
        }

        // Atualiza o espacamento entre obst�culos
        if (espacamento > 0) espacamento += -1;

        // Atualiza os pontos
        *pontos += 1;

        // Atualiza o level
        if (*pontos == 200 || *pontos == 400)
        {
            *level += 1;
            msg_do_level(E, *level);
        }

        // Atualiza o HUD
        gotoxy(E.HUD.x, E.HUD.y);
        printf("Level: %d    Life: %d    Score: %d", *level, *vida, *pontos);

    } // Fim do while (game_loop)

} // Fim da fun��o game_loop()


void play_game(janela J)
{
    short level = 1;
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

    desenha_cenario(J, E, level, vida, pontos); // Desenha o HUD da gameplay

    msg_do_level(E, 1); // Desenha mensagem do inicio do jogo

    game_loop(E, &level, &vida, &pontos); // Inicia a gameplay

}


void menu(janela J)
{
    // Vari�veis
    char *play_msg = "JOGAR";
    char *rank_msg = "Rank dos jogadores";
    char *exit_msg = "Sair do jogo";
    char input = -1; // Cont�m o input do usu�rio

    bool novo_frame = true;
    bool menu = true;

    coordenadas info = {J.linhas/2, J.colunas/8};
    coordenadas seta = {J.linhas/2, J.colunas/2 -strlen(play_msg) -4};
    coordenadas play = {J.linhas/2, J.colunas/2 -strlen(play_msg)/2};
    coordenadas rank = {J.linhas/2 +2, J.colunas/2 -strlen(rank_msg)/2};
    coordenadas exit = {J.linhas/2 +4, J.colunas/2 -strlen(exit_msg)/2};


    // Desenha o t�tulo do jogo
    gotoxy(J.linhas/4, J.colunas/2 -5);
    printf("Moon Buggy");


    //Desenha os controles/informa��es do jogo
    gotoxy(info.x, info.y);
    printf("Mover seta: W(cima) S(baixo)");

    gotoxy(info.x +1, info.y);
    printf("Selecionar: espaco(escolher)");


    // Desenha a seta de sele��o -->
    gotoxy(seta.x, seta.y);
    printf("-->");


    // Desenha o bot�o PLAY
    gotoxy(play.x, play.y);
    printf("%s", play_msg);


    //Desenha o bot�o RANK
    gotoxy(rank.x, rank.y);
    printf("%s", rank_msg);


    //Desenha o bot�o EXIT
    gotoxy(exit.x, exit.y);
    printf("%s", exit_msg);


    while (menu)
    {
        // Pega input do usu�rio caso uma tecla seja precionada
        if ( kbhit() ) input = getch();

        // Processa o input
        switch (input)
        {
        case 'w':
            if (seta.x -2 >= J.linhas/2)
            {
                // Apaga a seta atual
                gotoxy(seta.x, seta.y);
                printf("   ");

                seta.x += -2; // Movimenta a seta para cima
                if (seta.x == play.x)
                {
                    seta.y = play.y -4;
                }
                else if(seta.x == rank.x)
                {
                    seta.y = rank.y -4;
                }
                else if(seta.x == exit.x)
                {
                    seta.y =  exit.y -4;
                }

                novo_frame = true; // Re-desenha a seta para cima
            }
            break;

        case 's':
            if (seta.x +2 <= J.linhas/2 +4)
            {
                // Apaga a seta atual
                gotoxy(seta.x, seta.y);
                printf("   ");

                seta.x += 2; // Movimenta a seta para baixo
                if (seta.x == play.x)
                {
                    seta.y = play.y -4;
                }
                else if(seta.x == rank.x)
                {
                    seta.y = rank.y -4;
                }
                else if(seta.x == exit.x)
                {
                    seta.y =  exit.y -4;
                }

                novo_frame = true; // Re-desenha a seta para baixo
            }
            break;

        case ' ': // espa�o = enter

            if (seta.x == play.x)
            {
                system("cls");
                play_game(J);
            }
            else if (seta.x == rank.x)
            {
                system("cls");
                show_rank(J);

                // Re-desenha o menu
                gotoxy(info.x, info.y);
                printf("Mover seta: W(cima) S(baixo)");
                gotoxy(info.x +1, info.y);
                printf("Selecionar: espaco(escolher)");
                gotoxy(seta.x, seta.y);
                printf("-->");
                gotoxy(play.x, play.y);
                printf("%s", play_msg);
                gotoxy(rank.x, rank.y);
                printf("%s", rank_msg);
                gotoxy(exit.x, exit.y);
                printf("%s", exit_msg);
            }
            else if (seta.x == seta.x)
            {
                menu = false;
            }
            break;


        } // Fim do switch (input)

        // Renderiza as mudan�as
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

} // Fim da fun��o menu()


void init_game(janela J)
{
    menu(J);
}


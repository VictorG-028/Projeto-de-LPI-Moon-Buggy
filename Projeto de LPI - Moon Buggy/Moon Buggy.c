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
 * Eixo X é VERTICAL, ou seja representa as linhas (altura)
 * Eixo Y é HORIZONTAL, ou seja representa as colunas (distância da borda esquerda)
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


void ler_txt(short pontuacoes[100], char nicks[100][4])
{
    // Variáveis
    int i;

    FILE *f = fopen("rank.txt", "r");

    for(i = 0; i < 100; i++)
    {
        fscanf(f,"%hi,%s ;", &pontuacoes[i], nicks[i]);
    }

    fclose(f);

} // Fim da função ler_txt()


void salvar_txt(short pontuacoes[100], char nicks[100][4])
{
    // Variáveis
    int i;

    FILE *f = fopen("rank.txt", "w");

    for(i = 0; i < 100; i++)
    {
        fprintf(f,"%hi,%s ;", pontuacoes[i], nicks[i]);
    }

    fclose(f);

} // Fim da função salvar_txt()


void preencher_vetor(short tam, short vetor[tam], short num)
{
    // Variáveis
    int i;

    for (i = 0; i < tam; i++)
    {
        vetor[i] = num;
    }

} // Fim da função preencehr_vetor()


void preencher_nicks(char nicks[100][4])
{
    // Variáveis
    int i;

    for(i = 0; i < 100; i++)
    {
        strcpy(nicks[i], "XXX");
    }

} // Fim da função preencher_nicks()


void ordenar_vetor(short *vetor, short tam, char nicks[100][4])
{
    // Variáveis
    short i, j, aux;
    char temp[4];

    // Algoritmo bubble sort
    for(i = 0; i < tam; i++)
    {
        for(j = 0; j < i; j++)
        {
            if(vetor[i] > vetor[j])
            {
                aux = vetor[i];
                strcpy(temp, nicks[i]);

                vetor[i] = vetor[j];
                strcpy(nicks[i], nicks[j]);

                vetor[j] = aux;
                strcpy(nicks[j], temp);
            }
        }
    }

} // Fim da função ordenar_vetor()


void show_rank(janela J, short pontuacoes[100], char nicks[100][4])
{
    // variáveis
    int i;

    system("color 3");

    gotoxy(J.linhas/4, J.colunas/2 -strlen("TOP 10")/2);
    printf("Rank 10");

    for(i = 0; i < 10; i++)
    {
        gotoxy(J.linhas/4 +2+i, J.colunas/2 -strlen("Rank 10")/2);
        printf("%i\t%c%c%c", pontuacoes[i], nicks[i][0], nicks[i][1], nicks[i][2]);
    }

    getch();
    system("cls");
    system("color 4");

} // Fim da função show_rank()


void gravar_pontuacao(short pontos, char nick[4], short pontuacoes[100], char nicks[100][4])
{
    // variáveis
    short i, menor_pontuacao = 0, aux = 0;
    bool tem_vaga = false;

    // encontra o próxio espaço livre nas pontuações
    for(i = 0; i < 100; i++)
    {
        if (pontuacoes[i] == 0)
        {
            tem_vaga = true;
            pontuacoes[i] = pontos;
            strcpy(nicks[i], nick);
            break;
        }

        // encontra a menor pontuação no vetor
        if (i == 0)
        {
            menor_pontuacao = pontuacoes[i];
        }
        else if(menor_pontuacao > pontuacoes[i])
        {
            menor_pontuacao = pontuacoes[i];
            aux = i;
        }
    }

    if (!tem_vaga)
    {
        pontuacoes[aux] = pontos;
        strcpy(nicks[aux], nick);
    }

} // Fim da função gravar_pontuação()


void desenha_cenario(janela J, elementos E, short level, short vida, short pontos)
{
    // Variáveis
    int contador = 0;

    // Desenha a estrada
    gotoxy(E.estrada.x, E.estrada.y);
    do
    {
        printf("#");
        contador++;

    } while (contador < J.colunas);
    contador = 0; // Reseta o contador

    gotoxy(E.estrada.x +1, E.estrada.y);
    do
    {
        printf("#");
        contador++;

    } while (contador < J.colunas);
    contador = 0; // Reseta o contador

    // Apaga as 3 linhas acima da estrada (reset dps que morre)
    gotoxy(E.estrada.x -1, E.estrada.y);
    do
    {
        printf(" ");
        contador++;

    } while (contador < J.colunas);
    contador = 0; // Reseta o contador

    gotoxy(E.estrada.x -2, E.estrada.y);
    do
    {
        printf(" ");
        contador++;

    } while (contador < J.colunas);
    contador = 0; // Reseta o contador

    gotoxy(E.estrada.x -3, E.estrada.y);
    do
    {
        printf(" ");
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

} // Fim da função desenhar_cenario()


void pause(elementos E, short *vidas, bool *game_loop, bool *debug_mode)
{
    // variáveis
    char *continuar_msg = "Continuar ";
    char *salvar_msg = "Salvar e sair ";
    char *debug_msg = "Debug:";
    char input = -1;

    bool pausa = true;
    bool novo_frame = false;

    int i = 0;

    coordenadas seta = {E.info.x, E.info.y -strlen(continuar_msg)/2 -4};
    coordenadas continuar = {E.info.x, E.info.y -strlen(continuar_msg)/2};
    coordenadas salvar = {E.info.x +2, E.info.y -strlen(salvar_msg)/2};
    coordenadas debug = {E.info.x +4, E.info.y -strlen(debug_msg)/2 - (debug_mode ? 2 : 3) };

    // Desenha as opções
    gotoxy(continuar.x, continuar.y);
    printf("%s", continuar_msg);

    gotoxy(salvar.x, salvar.y);
    printf("%s", salvar_msg);

    gotoxy(debug.x, debug.y);
    printf("%s %s", debug_msg, *debug_mode ? "True" : "False");

    // Desenha a seta
    gotoxy(seta.x, seta.y);
    printf("-->");

    while (pausa)
    {
        // Pega input do usuário caso uma tecla seja precionada
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

                // Movimenta a seta para trás da palavra
                if (seta.x == E.info.x +2)
                {
                    seta.y = continuar.y -4;
                }
                else if (seta.x == E.info.x +4)
                {
                    seta.y = salvar.y -4;
                }

                seta.x += -2; // Movimenta a seta para cima

                novo_frame = true; // Re-desenha a seta
            }
            break;

        case 's':
            if(seta.x +2 <= E.info.x +4)
            {
                // Apaga a seta atual
                gotoxy(seta.x, seta.y);
                printf("   ");

                // Movimenta a seta para trás da palavra
                if (seta.x == E.info.x)
                {
                    seta.y = salvar.y -4;
                }
                else if (seta.x == E.info.x +2)
                {
                    seta.y = debug.y -4;
                }

                seta.x += +2; // Movimenta a seta para baixo

                novo_frame = true; // Re-desenha a seta
            }
            break;

        case ' ':
            if (seta.x == E.info.x)
            {
                pausa = false;
            }
            else if (seta.x == E.info.x +2)
            {
                pausa = false;
                *vidas = 0;
                *game_loop = false;
            }
            else if (seta.x == E.info.x +4)
            {
                if (*debug_mode) // Se true
                {
                    *debug_mode = false; // Então fica false
                }
                else // Senão é true
                {
                    *debug_mode = true; // Então fica true
                }

                novo_frame = true;
            }
            break;

        } // Fim d switch (input)

        // Renderiza as mudanças do input
        if (novo_frame)
        {
            // Re-desenha a seta
            gotoxy(seta.x, seta.y);
            printf("-->");

            // Atualiza o debug_mode visulamente
            gotoxy(debug.x, debug.y);
            printf("%s %s", debug_msg, *debug_mode ? "True " : "False");

            novo_frame = false;
        }

        // Limpa o input
        input = -1;

    } // Fim do while (pause)

    // Apaga as mensagens do menu da pausa
    gotoxy(seta.x, seta.y);
    printf("   ");

    // Apaga as informações do debug mode
    // Caso esteja true, as informações serã re-colocadas no game_loop()
    gotoxy(1, 1);
    printf("       ");
    gotoxy(2, 1);
    printf("                             ");
    gotoxy(3, 1);
    printf("                             ");
    gotoxy(4, 1);
    printf("                             ");
    gotoxy(5, 1);
    printf("                             ");
    gotoxy(6, 1);
    printf("                             ");

    gotoxy(continuar.x, continuar.y);
    do { printf(" "); i++; } while (i < strlen(continuar_msg) );

    i = 0; // Reseta o contador

    gotoxy(salvar.x, salvar.y);
    do { printf(" "); i++; } while (i < strlen(salvar_msg) );

    i = 0; // Reseta o contador

    gotoxy(debug.x, debug.y);
    do { printf(" "); i++; } while (i < strlen(debug_msg) + (*debug_mode ? 5 : 6) );

} // Fim da função pause()


void msg_do_level(elementos E, short level_atual)
{
    // Variáveis
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
        msg_2 = "Pressione <Z> para continuar";
    }
    else if (level_atual == 3)
    {
        msg_1 = "Pilhas de pedra e buracos pequenos a frente!!!";
        msg_2 = "Atire com <Z> para continuar!";
    }
    else
    {
        // Esse trecho não deve ser executado, mas se for, então existe algum erro
        msg_1 = "ERRO";
        msg_2 = "ERRO";
    }

    // Calcula as coodenadas com base na msg que vai ser escrita
    _msg_1.x = E.info.x;
    _msg_1.y = E.info.y -strlen(msg_1)/2;

    _msg_2.x = E.info.x +1;
    _msg_2.y = E.info.y -strlen(msg_2)/2;

    // Desenha informações adicionais do level 1
    gotoxy(_msg_1.x, _msg_1.y);
    printf("%s", msg_1);

    gotoxy(_msg_2.x, _msg_2.y);
    printf("%s", msg_2);

    // Espera o input
    if(level_atual == 1)
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

} // Fim da função msg_do_level();


void logica_pular(elementos *E, bool *pular, bool *subir, bool *descer, short *tempo_ar, bool debug_mode)
{
    // Variáveis
    short altura_max = 4;

    if (debug_mode)
    {
        gotoxy(5, 1);
        printf("Tempo no ar: %i", *tempo_ar);
    }

    // Atualiza a altura
    if (*subir)
    {
        if (E->buggy.x < E->estrada.x -altura_max) // Limita a altura max de subida
        {
            if (*tempo_ar == 0)
            {
                // Inverte subir e descer
                *subir = false;
                *descer = true;

                *tempo_ar = 2;
            }
            else
            {
                *tempo_ar -= 1;
            }
        }
        else
        {
            E->buggy.x -= 1; // Sobe o buggy
        }
    }
    else if (*descer)
    {
        if (E->buggy.x > E->estrada.x -3) // Limita a descida até o chão
        {
            // Finalização da animação do pulo
            *descer = false;
            *pular = false;
        }
        else if(*tempo_ar > 0 && E->buggy.x == E->estrada.x -altura_max +1)
        {
            *tempo_ar -= 1;
        }
        else
        {
            E->buggy.x += +1; // Desce o buggy
        }
    }

} // Fim da função logica_pular()


void desenhar_pulo(elementos E,bool subir, short *frame_cabine, short *frame_chassi, short *frame_roda)
{
    // Variáveis
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

    gotoxy(E.buggy.x +2, E.buggy.y); // Antiga posição das rodas
    if (subir || E.buggy.x != E.estrada.x -2) printf("       "); // Remove as rodas da tela

    gotoxy(E.buggy.x -1, E.buggy.y); // Antiga posição do chassi
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

} // Fim da função desenhar_pulo()


void gerar_buraco_pequeno(short *buracos, short *largura_buraco, short max, short *espacamento, bool debug_mode)
{
    // Variáveis
    short i;

    if (debug_mode)
    {
        gotoxy(2, 1);
        printf("Gerou buraco pequeno");
    }

    // Só gera um obstáculo após espaçamento chegar a 0 novamente
    if (*espacamento == 0)
    {
        // Busca no vetor o índice de um obstáculo que não está sendo utilizado
        for(i = 0; i < max; i++)
        {
            // Caso acho algum espaço livre, cria um obstáculo
            if (buracos[i] == -1)
            {
                buracos[i] = 0;
                largura_buraco[i] = 1;
                *espacamento = rand() % 11 +15; // Gera números de 15 à 25
                break;
            }
        }
    }

} // Fim da função gerar_buracos_pequenos()


void gerar_buraco_grande(short *buracos, short *largura_buraco, short max, short *espacamento, bool debug_mode)
{
    // Variáveis
    short tamanho;
    int i;

    // Só gera um obstáculo após espaçamento chegar a 0 novamente
    if (*espacamento == 0)
    {
        // Define o tamanho do buraco aleatoriamente
        tamanho = rand() % 4 +1; // Varia de 1 à 4

        if (debug_mode)
        {
            gotoxy(2, 1);

            if (tamanho == 1)
            {

                printf("Gerou buraco pequeno");
            }
            else
            {
                printf("Gerou buraco grande");
            }
        }

        for(i = 0; i < max; i++)
        {
            if (buracos[i] == -1)
            {
                buracos[i] = 0;
                largura_buraco[i] = tamanho;
                *espacamento = rand() % 16 +15; // Gera números de 15 à 30
                break;
            }
        }

    }

} // Fim da função gerar_buracos_grandes()


void gerar_pedra(short *pedras, short* altura_pedra, short max, short *espacamento, bool debug_mode)
{
    // Variáveis
    short i;

    if (debug_mode)
    {
        gotoxy(2, 1);
        printf("Gerou pedra         ");
    }

    // Só gera um obstáculo após espaçamento chegar a 0 novamente
    if (*espacamento == 0)
    {
        // Busca no vetor o índice de um obstáculo que não está sendo utilizado
        for(i = 0; i < max; i++)
        {
            // Caso exista algum espaço livre, cria um obstáculo
            if (pedras[i] == -1)
            {
                pedras[i] = 0;
                altura_pedra[i] = rand() % 3 +1;// Gera pilhas de 1 à 3 de altura
                *espacamento = rand() % 11 +10; // Gera números de 10 à 20
                break;
            }
        }
    }

} // Fim da função gerar_pedras()


void atualizar_buracos(elementos E, short *buracos, short *largura_buraco, short max_buracos, short limite_direita)
{
    // Variáveis
    int i, j;

    // Atualiza a posição dos buracos
    for(i = 0; i < max_buracos; i++)
    {
        // Atualiza a posição se o buraco existir
        if (buracos[i] != -1) buracos[i] += 1;

        // Retira o buraco ao chegar no fim da estrada
        if (buracos[i] == limite_direita)
        {
            for(j = 0; j < largura_buraco[i]; j++)
            {
                gotoxy(E.estrada.x, buracos[i] -1-j);
                printf("#");
            }

            // Reseta o buraco
            buracos[i] = -1;
        }
    }

} // Fim da função atualizar_buracos()


void atualiza_pedras(elementos E, short *pedras, short *altura_pedras, short max_pedras, short limite_direita)
{
    // variáveis
    int i, j;

    for(i = 0; i < max_pedras; i++)
    {
        // Atualiza a posição se a pedra existir
        if (pedras[i] != -1) pedras[i] += 1;


        // Retira a pedra ao chegar no fim
        if (pedras[i] == limite_direita)
        {
            // Apaga toda a pilha de pedras
            for(j = 0; j < altura_pedras[i]; j++)
            {
                gotoxy(E.estrada.x -1 -j, pedras[i]-1);
                printf("E");
            }

            pedras[i] = -1;
        }
    }

} // Fim da função atualizar_pedras()


void desenhar_buracos(elementos E, short *buracos, short *largura_buraco, short max_buracos)
{
    // Variáveis
    int i;

    for(i = 0; i < max_buracos; i++)
    {
        // Desenha apenas os buracos existentes
        if (buracos[i] != -1)
        {
            if (largura_buraco[i] == 1)
            {
                gotoxy(E.estrada.x, buracos[i]);
                printf(" ");

                // Apaga o rastro do buraco
                if (buracos[i] > 0)
                {
                    gotoxy(E.estrada.x, buracos[i] -1);
                    printf("#");
                }
            }
            else if (largura_buraco[i] == 2)
            {
                gotoxy(E.estrada.x, buracos[i]);
                printf(" ");

                gotoxy(E.estrada.x, buracos[i] -1);
                printf(" ");

                // Apaga o rastro do buraco
                if (buracos[i] > 0)
                {
                    gotoxy(E.estrada.x, buracos[i] -2);
                    printf("#");
                }
            }
            else if (largura_buraco[i] == 3)
            {
                gotoxy(E.estrada.x, buracos[i]);
                printf(" ");

                gotoxy(E.estrada.x, buracos[i] -1);
                printf(" ");

                gotoxy(E.estrada.x, buracos[i] -2);
                printf(" ");

                // Apaga o rastro do buraco
                if (buracos[i] > 0)
                {
                    gotoxy(E.estrada.x, buracos[i] -3);
                    printf("#");
                }
            }
            else if (largura_buraco[i] == 4)
            {
                gotoxy(E.estrada.x, buracos[i]);
                printf(" ");

                gotoxy(E.estrada.x, buracos[i] -1);
                printf(" ");

                gotoxy(E.estrada.x, buracos[i] -2);
                printf(" ");

                gotoxy(E.estrada.x, buracos[i] -3);
                printf(" ");

                // Apaga o rastro do buraco
                if (buracos[i] > 0)
                {
                    gotoxy(E.estrada.x, buracos[i] -4);
                    printf("#");
                }
            }
        }
    }

} // Fim da função desenhar_buracos()


void desenhar_pedras(elementos E, short *pedras, short *altura_pedra, short max_pedras)
{
    // Variáveis
    int i;

    for(i = 0; i < max_pedras; i++)
    {
        // Desenha apenas as pedras existentes
        if (pedras[i] != -1)
        {
            if (altura_pedra[i] == 1)
            {
                gotoxy(E.estrada.x -1, pedras[i]);
                printf("#");

                // Apaga o ratro da pedra
                if (pedras[i] > 0)
                {
                    gotoxy(E.estrada.x -1, pedras[i] -1);
                    printf(" ");
                }
            }
            else if(altura_pedra[i] == 2)
            {
                gotoxy(E.estrada.x -1, pedras[i]);
                printf("#");

                gotoxy(E.estrada.x -2, pedras[i]);
                printf("#");

                // Apaga o ratro da pedra
                if (pedras[i] > 0)
                {
                    gotoxy(E.estrada.x -1, pedras[i] -1);
                    printf(" ");

                    gotoxy(E.estrada.x -2, pedras[i] -1);
                    printf(" ");
                }
            }
            else if (altura_pedra[i] == 3)
            {
                gotoxy(E.estrada.x -1, pedras[i]);
                printf("#");

                gotoxy(E.estrada.x -2, pedras[i]);
                printf("#");

                gotoxy(E.estrada.x -3, pedras[i]);
                printf("#");

                // Apaga o ratro da pedra
                if (pedras[i] > 0)
                {
                    gotoxy(E.estrada.x -1, pedras[i] -1);
                    printf(" ");

                    gotoxy(E.estrada.x -2, pedras[i] -1);
                    printf(" ");

                    gotoxy(E.estrada.x -3, pedras[i] -1);
                    printf(" ");
                }
            }
        }
    }

} // Fim da função desenhar_pedras()


void morte_buraco(elementos E, short buraco, short largura_buraco, short *buracos, short max)
{
    // Variáveis
    int i, j;

    coordenadas roda = {E.buggy.x +1, E.buggy.y -2};

    // Essa parte não está funcionando
    /*
    int marcador = 0;

    char *cima  = "  ___  ";
    char *baixo = "cnOMMnb";

    // Derruba o buggy onde tem buraco

    for(i = 0; i < 7; i++)
    {
        if (E.buggy.y+i == buraco)
        {
            marcador = i;
        }
    }

    if (marcador > 0)
    {
        for(i = 0; i < buraco -largura_buraco +1; i++)
        {
            gotoxy(E.buggy.x, E.buggy.y +i);
            printf("%c", cima[i]);

            gotoxy(E.buggy.x +1, E.buggy.y +i);
            printf("%c", baixo[i]);
        }
    }

    for(i = 0; i < largura_buraco; i++)
    {
        if (buraco < E.buggy.y +7)
        {
            gotoxy(E.buggy.x +1, E.buggy.y +buraco);
            printf("%c", cima[buraco]);

            gotoxy(E.buggy.x +2, E.buggy.y +buraco);
            printf("%c", baixo[buraco]);
        }

        buraco++;
    }

    if (1)
    {
        for(i = buraco; i < 7 ; i++)
        {
            gotoxy(E.buggy.x, E.buggy.y +i);
            printf("%c", cima[i]);

            gotoxy(E.buggy.x +1, E.buggy.y +i);
            printf("%c", baixo[i]);
        }
    }
    */

    gotoxy(E.buggy.x, E.buggy.y);
    printf("   ___  ");

    gotoxy(E.buggy.x +1, E.buggy.y);
    printf("cnOMMnb");

    //gotoxy(E.estrada.x, buraco);
    //printf("A");

    // Frames da animação
    //  <--7-6--5-4-3--2-1
    /*
             o  o   o  o     ___
           o      o      o cnOMMnb
    */

    // Animação da roda
    for(i = 1; i <= 7; i++)
    {
        // Desenha novo frame da roda
        gotoxy(roda.x, roda.y);
        printf("o");

        Sleep(240);

        // Apaga o ultimo frame da roda
        if (i != 7) // Se o frame for diferente do último
        {
            gotoxy(roda.x, roda.y);
            printf(" ");
        }

        // Verifica se a roada está pulando no chão, caso contrário ela para de pular
        if (i == 4 || i == 7) // Se o frame for 4 ou 7
        {
            for(j = 0; j < max; j++)
            {
                if (roda.y == buracos[j]) // Se a roda está acima de um buraco
                {
                    // Apaga a roda da tela
                    gotoxy(roda.x, roda.y);
                    printf(" ");

                    // Redesenha no buraco
                    gotoxy(roda.x +1, roda.y);
                    printf("o");

                    i = 8; // Termina o loop i
                    break; // Termina o loop j
                }
            }
        }


        // Atualiza a posição da roda
        roda.y -= 2;

        if (i == 1 || i == 2 || i == 4 || i == 5)
        {
            roda.x = E.buggy.x;
        }
        else
        {
            roda.x = E.buggy.x +1;
        }
    }

    Sleep(240);

} // Fim da função morte_buraco()


void verificar_colisao_buraco(elementos E, short *buracos, short *largura_buraco, short max, short *vida, bool *game_loop)
{
    // Variáveis
    short hitbox = E.buggy.y;
    short B;
    int i, j, k;

    for(i = 0; i < max; i++) // Testa cada buraco
    {
        if (buracos[i] != -1)
        {
            B = buracos[i];

            for(j = 0; j < 7; j++) // Testa para cada parte debaixo do buggy para cada buraco
            {
                for(k = 0; k < largura_buraco[i]; k++) // Testa o comprimento do buraco para cada buraco em cada parte do buggy
                {
                    if (hitbox +j == B -k)
                    {
                        *vida -= 1; // Perde 1 vida
                        *game_loop = false; // Acaba a iteração da vida
                        morte_buraco(E, B, largura_buraco[i], buracos, max);

                        i = max;
                        j = 7;
                        break;
                    }
                }
            }
        }
    }

} // Fim da função verificar_colisão_buraco()


void morte_pedra(elementos E, short pedra, short altura_pedra, short max_pedras)
{
    // Frame da morte ao bater numa pedra
    gotoxy(E.buggy.x, E.buggy.y+1);
    printf("cn_  _ ");

    gotoxy(E.buggy.x +1, E.buggy.y+1);
    printf("OMM_nb ");

    Sleep(1500);

} // Fim da função morte_pedra()


void verificar_colisao_pedra(elementos E, short *pedras, short *altura_pedra, short max, short *vida, bool *game_loop)
{
    // variáveis
    int i;

    for(i = 0; i < max; i++)
    {
        // Se pedra existe
        if (pedras[i] != -1)
        {
            if (pedras[i] == E.buggy.y && E.buggy.x == E.estrada.x-2)
            {
                *vida -= 1; // Perde 1 vida
                *game_loop = false; // Acaba a iteração da vida
                morte_pedra(E, pedras[i], altura_pedra[i], max);
            }
        }
    }
} // Fim da função verificar_colisão_pedra()


void atualizar_tiro(elementos E, short *tiros, short max_tiros, short *pedras, short *altura_pedra, short max_pedras, bool debug_mode)
{
    // variáveis
    int i, j;

    for(i = 0; i < max_tiros; i++)
    {
        // Se o tiro existir
        if (tiros[i] > -1)
        {
            tiros[i] -= 1;// Atualiza a posição

            if (tiros[i] == E.estrada.y-1)
            {
                gotoxy(E.estrada.x-1, tiros[i]);
                printf(" ");

                tiros[i] = -1;
            }

            // Colisão do tiro com a pedra
            for(j = 0; j < max_pedras; j++)
            {
                // Se tiro tocar na pedra
                if (tiros[i] == pedras[j] || tiros[i] == pedras[j]-1)
                {
                    // Reseta o tiro e a pedra (pedra redesenhada)
                    //gotoxy(E.estrada.x -1, pedras[i]);
                    //if (tiros[i] > -1) printf("C");
                    //gotoxy(E.estrada.x -1, pedras[i]-1);
                    //if (tiros[i] > -1) printf("%i", pedras[i]);

                    tiros[i] = -1;

                    // Pedra diminui de tamanaho ou é completamente destruida
                    if (altura_pedra[j] == 1)
                    {
                        gotoxy(E.estrada.x -1, pedras[j]);
                        printf(" ");

                        altura_pedra[j] = -1;
                        pedras[j] = -1;
                    }
                    else if (altura_pedra[j] > 1)
                    {
                        gotoxy(E.estrada.x -altura_pedra[j], pedras[j]);
                        printf(" ");

                        altura_pedra[j] -= 1;
                    }
                }
            }
            if (debug_mode)
            {
                gotoxy(6, 1);
                printf("1:%i | 2:%i", tiros[0], tiros[1]);
            }

        }
    }



    // Desenha as mudanças do tiro
    for(i = 0; i < max_tiros; i++)
    {
        if (tiros[i] != -1)
        {
            gotoxy(E.estrada.x -1, tiros[i]+1);
            printf(" ");

            gotoxy(E.estrada.x -1, tiros[i]);
            printf("<");
        }
    }

} // Fim da função atualizar_tiro()


void game_loop(elementos E, short *level,short *vida, short *pontos)
{
    // Variáveis
    short frame_roda = 0;
    short frame_chassi = 0;
    short frame_cabine = 0;

    short max_buracos = 10; // Máx de 10 buracos pequenos
    short buracos[max_buracos];
    short largura_buraco[max_buracos]; // Guarda o tamanho de cada buraco
    // Ex: buracos[i] tem tamanho_buraco[i]

    short max_pedras  = 20; // Máx de 20 pilhas de pedras
    short pedras[max_pedras];
    short altura_pedra[max_pedras];  // Guarda a altura de cada pedra
    // Ex: pedras[i] tem tamanho_pedra[i]

    short espacamento = 0; // Espaço de estrada entre obstáculos
    short limite_direita = E.buggy.y*4/3;

    short tempo_ar = 0;

    short max_tiros = 2;
    short tiros[max_tiros];

    char roda[] = "\\-/|U"; // Charactéres da roda

    char input; // Contém o input do usuário

    bool pular = false, subir = false, descer = false;

    bool debug_mode = false;
    bool game_loop = true;

    // Preenche os vetores de buraco e pedra com um número em todas as posições
    preencher_vetor(max_buracos, buracos, -1);
    preencher_vetor(max_pedras, pedras, -1);
    preencher_vetor(max_tiros, tiros, -1);

    preencher_vetor(max_buracos, largura_buraco, 1);
    preencher_vetor(max_pedras, altura_pedra, -1);

    while (game_loop)
    {
        // Pega input do usuário caso uma tecla seja precionada
        Sleep(100);
        if ( kbhit() ) input = getch();

        // Processa o input
        switch (input)
        {
        case ' ':
            if(pular == false)
            {
                tempo_ar = 2;
                pular = true;
                subir = true;
            }

            break;

        case 'z':
            if(*level >= 3)
            {
                // Gera um tiro caso existe um slot livre
                if (tiros[0] == -1)
                {
                    tiros[0] = E.buggy.y -2;
                }
                else if(tiros[1] == -1)
                {
                    tiros[1] = E.buggy.y -2;
                }
            }

            break;

        case 'p':
            if (debug_mode)
            {
                gotoxy(1, 1);
                printf("Pausado");
            }

            pause(E, vida, &game_loop, &debug_mode);

            break;

        } // Fim do switch input

        if (game_loop)
        {
            // Renderiza as mudanças do input
            if (pular)
            {
                // Lógica do pulo
                logica_pular(&E, &pular, &subir, &descer, &tempo_ar, debug_mode);

                // Desenha o carro enquanto pula
                desenhar_pulo(E, subir, &frame_chassi, &frame_cabine, &frame_roda);
            }
            else // Senão está pulando, está rodando a roda
            {
                // Ajusta o frame da cabine e do chassi após o pulo
                gotoxy(E.buggy.x, E.buggy.y);
                printf("   Omn");

                // Animação da roda do carro
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

            // Gera os obstáculos de acordo com o level e pontos
            // Os pontos marcam o intervalo sem obstáculos entre levels
            if (*level == 1 && *pontos > 5)
            {
                gerar_buraco_pequeno(buracos, largura_buraco, max_buracos, &espacamento, debug_mode);
            }
            else if (*level == 2 && *pontos > 230)
            {
                // Pode gerar buracos pequenos ou grandes
                gerar_buraco_grande(buracos, largura_buraco, max_buracos, &espacamento, debug_mode);
            }
            else if (*level == 3 && *pontos > 430)
            {
                if (rand() % 2)
                {
                    gerar_buraco_pequeno(buracos, largura_buraco, max_pedras, &espacamento, debug_mode);
                }
                else
                {
                    gerar_pedra(pedras, altura_pedra, max_pedras, &espacamento, debug_mode);
                }
            }

            // Atualiza a posição y (coluna) do tiro
            atualizar_tiro(E, tiros, max_tiros, pedras, altura_pedra, max_pedras, debug_mode);

            // Atualiza a posição y (coluna) dos buracos
            atualizar_buracos(E, buracos, largura_buraco, max_buracos, limite_direita);

            // Atualiza a posição y (coluna) das pilhas de pedra
            atualiza_pedras(E, pedras, altura_pedra, max_pedras, limite_direita);

            // Desenha os buracos no terminal
            desenhar_buracos(E, buracos, largura_buraco, max_buracos);

            // Desenha as pedras no terminal
            desenhar_pedras(E, pedras, altura_pedra, max_pedras);

            // Confere se o carro não caio ou bateu
            if (debug_mode)
            {
                gotoxy(3, 1);
                printf("Buggy linha: %d", E.buggy.x);
                gotoxy(4, 1);
                printf("Buggy coluna: %d", E.buggy.y);
            }

            if (!pular)
            {
                verificar_colisao_buraco(E, buracos, largura_buraco, max_buracos, vida, &game_loop);
            }

            if (game_loop)
            {
                verificar_colisao_pedra(E, pedras, altura_pedra, max_pedras, vida, &game_loop);
            }

            // Atualiza o espaçamento entre obstáculos
            if (espacamento > 0) espacamento -= 1;

            // Atualiza os pontos
            *pontos += 1;

            // Atualiza o level
            if ( (*pontos == 200 || *pontos == 400) && game_loop)
            {
                *level += 1;
                msg_do_level(E, *level);
            }

            // Atualiza o HUD
            gotoxy(E.HUD.x, E.HUD.y);
            printf("Level: %d    Life: %d    Score: %d", *level, *vida, *pontos);
        }

    } // Fim do while (game_loop)

} // Fim da função game_loop()


void play_game(janela J, short pontuacoes[100], char nicks[100][4])
{
    short level = 1;
    short vida = 3;
    short pontos = 0;

    char nick[4];

    elementos E; // Guarda as coordenadas dos elementos da janela

    E.estrada.x = J.linhas*3/4;
    E.estrada.y = 0;

    E.buggy.x = J.linhas*3/4 -2;
    E.buggy.y = J.colunas*3/4;

    E.HUD.x = J.linhas*3/4 +2;
    E.HUD.y = J.colunas/2 -15;

    E.info.x = J.linhas*3/4 -6;
    E.info.y = J.colunas/2;

    do {
        desenha_cenario(J, E, level, vida, pontos); // Desenha o HUD da gameplay

        msg_do_level(E, level); // Desenha mensagem do inicio do jogo

        game_loop(E, &level, &vida, &pontos); // Inicia a gameplay

    } while (vida > 0);

    // Atualiza info para centro da janela
    E.info.x = J.linhas/2;
    E.info.y = J.colunas/2;

    // Limpa tela
    system("cls");

    // Pede o nick do jogador atual
    fflush(stdin);
    gotoxy(E.info.x, E.info.y -strlen("Digite seu nick de 3 letras: ")/2 );
    printf("Digite seu nick de 3 letras: ");
    scanf("%s", nick);
    fflush(stdin);

    // Grava a pontuação em um vetor
    gravar_pontuacao(pontos, nick, pontuacoes, nicks);

    // Ordena o vetor
    ordenar_vetor(pontuacoes, 100, nicks);

    // MSG de aguardando
    gotoxy(E.info.x +1, E.info.y-strlen("Digite seu nick de 3 letras: ")/2 );
    printf("Aguarde o salvamento. . .");
    Sleep(750);


    system("cls");

} // Fim da função play_game()


void menu(janela J)
{
    // Variáveis
    char *play_msg = "JOGAR";
    char *rank_msg = "Rank dos jogadores";
    char *exit_msg = "Sair do jogo";
    char input = -1; // Contém a tecla apertada

    bool novo_frame = true;
    bool menu = true;

    coordenadas info = {J.linhas/2, J.colunas/8};
    coordenadas seta = {J.linhas/2, J.colunas/2 -strlen(play_msg) -4};
    coordenadas play = {J.linhas/2, J.colunas/2 -strlen(play_msg)/2};
    coordenadas rank = {J.linhas/2 +2, J.colunas/2 -strlen(rank_msg)/2};
    coordenadas exit = {J.linhas/2 +4, J.colunas/2 -strlen(exit_msg)/2};

    short pontuacoes[100]; // Contém as pontuações dos jogadores
    char nicks[100][4]; // Matriz que contèm os nicks dos jogadores

    // Preenche o vetor de pontuações com 0 (vazio)
    preencher_vetor(100, pontuacoes, 0);

    // preenceh os nicks com "   "
    preencher_nicks(nicks);

    // Carrega o arquivo
    ler_txt(pontuacoes, nicks);

    // Desenha o título do jogo
    gotoxy(J.linhas/4, J.colunas/2 -5);
    printf("  MOON");

    gotoxy(J.linhas/4 +1, J.colunas/2 -5);
    printf("     BUGGY");

    gotoxy(J.linhas/4 -2, J.colunas/2 -5);
    printf("+=-=-=-=-=+");

    gotoxy(J.linhas/4 +3, J.colunas/2 -5);
    printf("+=-=-=-=-=+");


    //Desenha os controles/informações do jogo
    gotoxy(info.x, info.y);
    printf("W(cima) S(baixo)");

    gotoxy(info.x +1, info.y);
    printf("espaco(escolher)");


    // Desenha a seta de seleção -->
    gotoxy(seta.x, seta.y);
    printf("-->");


    // Desenha o botão PLAY
    gotoxy(play.x, play.y);
    printf("%s", play_msg);


    //Desenha o botão RANK
    gotoxy(rank.x, rank.y);
    printf("%s", rank_msg);


    //Desenha o botão EXIT
    gotoxy(exit.x, exit.y);
    printf("%s", exit_msg);


    while (menu)
    {
        // Pega input do usuário caso uma tecla seja precionada
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

        case ' ': // espaço = enter

            if (seta.x == play.x)
            {
                system("cls");
                play_game(J, pontuacoes, nicks);

                // Re-desenha o menu
                gotoxy(J.linhas/4, J.colunas/2 -5);
                printf("  MOON");
                gotoxy(J.linhas/4 +1, J.colunas/2 -5);
                printf("     BUGGY");
                gotoxy(J.linhas/4 -2, J.colunas/2 -5);
                printf("+=-=-=-=-=+");
                gotoxy(J.linhas/4 +3, J.colunas/2 -5);
                printf("+=-=-=-=-=+");
                gotoxy(info.x, info.y);
                printf("W(cima) S(baixo)");
                gotoxy(info.x +1, info.y);
                printf("espaco(escolher)");
                gotoxy(seta.x, seta.y);
                printf("-->");
                gotoxy(play.x, play.y);
                printf("%s", play_msg);
                gotoxy(rank.x, rank.y);
                printf("%s", rank_msg);
                gotoxy(exit.x, exit.y);
                printf("%s", exit_msg);
            }
            else if (seta.x == rank.x)
            {
                system("cls");
                show_rank(J, pontuacoes, nicks);

                // Re-desenha o menu
                gotoxy(J.linhas/4, J.colunas/2 -5);
                printf("  MOON");
                gotoxy(J.linhas/4 +1, J.colunas/2 -5);
                printf("     BUGGY");
                gotoxy(J.linhas/4 -2, J.colunas/2 -5);
                printf("+=-=-=-=-=+");
                gotoxy(J.linhas/4 +3, J.colunas/2 -5);
                printf("+=-=-=-=-=+");
                gotoxy(info.x, info.y);
                printf("W(cima) S(baixo)");
                gotoxy(info.x +1, info.y);
                printf("espaco(escolher)");
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
                salvar_txt(pontuacoes, nicks);
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

    printf("\n");

} // Fim da função menu()


void init_game(janela J)
{
    menu(J);

} // Fim da função init_game()


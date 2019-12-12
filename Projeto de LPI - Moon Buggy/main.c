#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <stdbool.h>
#include <windows.h>

#include "Moon Buggy.h"

int main()
{
    ////////////////////////

    CONSOLE_SCREEN_BUFFER_INFO console_info; // Contém informações do terminal do Code::Blocks

    // Preenche a variável console_info com as informações
    GetConsoleScreenBufferInfo( GetStdHandle(STD_OUTPUT_HANDLE), &console_info);

    short linha = console_info.srWindow.Bottom - console_info.srWindow.Top + 1;
    short coluna = console_info.srWindow.Right - console_info.srWindow.Left + 1;

    janela J = {linha, coluna}; // J = janela

    srand(time(NULL)); // inicia a seed de número aleatórios

    system("color 4"); // Muda a cor das letras para vermehlo

    ////////////////////////

    init_game(J);


    return 0;
}

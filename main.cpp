#include "raylib.h"
#include "snake.hpp"

int main() {
    InitWindow(COLUNAS * TAMANHO_CELULA, LINHAS * TAMANHO_CELULA, "Snake");
    SetTargetFPS(60);

    Jogo jogo;
    reiniciar(jogo);

    while (!WindowShouldClose()) {
        lerInput(jogo);
        atualizar(jogo);
        desenhar(jogo);
    }

    CloseWindow();
    return 0;
}
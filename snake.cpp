#include "snake.hpp"
#include "raylib.h"

Posicao sortearComida(const std::vector<Posicao>& cobra) {
    while (true) {
        Posicao p = { GetRandomValue(0, COLUNAS - 1), GetRandomValue(0, LINHAS - 1) };

        bool emCimaDaCobra = false;
        for (size_t i = 0; i < cobra.size(); i++) {
            if (cobra[i].x == p.x && cobra[i].y == p.y) {
                emCimaDaCobra = true;
            }
        }

        if (!emCimaDaCobra) {
            return p;
        }
    }
}

void desenharTextoCentralizado(const char* texto, int y, int tamanho, Color cor) {
    int largura = MeasureText(texto, tamanho);
    int x = (COLUNAS * TAMANHO_CELULA - largura) / 2;
    DrawText(texto, x, y, tamanho, cor);
}

void reiniciar(Jogo& jogo) {
    int cx = COLUNAS / 2;
    int cy = LINHAS / 2;
    jogo.cobra = {{cx, cy}, {cx - 1, cy}, {cx - 2, cy}};
    jogo.comida = sortearComida(jogo.cobra);
    jogo.acumulador = 0.0f;
    jogo.direcao = {1, 0};
    jogo.ultimoPasso = {1, 0};
    jogo.pontuacao = 0;
    jogo.gameOver = false;
    jogo.vitoria = false;
}

void lerInput(Jogo& jogo) {
    if (!jogo.gameOver) {
        if ((IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) && jogo.ultimoPasso.x != -1) {
            jogo.direcao = {1, 0};
        }
        if ((IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) && jogo.ultimoPasso.x != 1) {
            jogo.direcao = {-1, 0};
        }
        if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && jogo.ultimoPasso.y != 1) {
            jogo.direcao = {0, -1};
        }
        if ((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) && jogo.ultimoPasso.y != -1) {
            jogo.direcao = {0, 1};
        }
    } else {
        if (IsKeyPressed(KEY_R)) {
            reiniciar(jogo);
        }
    }
}

void atualizar(Jogo& jogo) {
    if (jogo.gameOver) return;

    jogo.acumulador += GetFrameTime();
    if (jogo.acumulador < INTERVALO) return;
    jogo.acumulador -= INTERVALO;

    Posicao novaCabeca = { jogo.cobra[0].x + jogo.direcao.x, jogo.cobra[0].y + jogo.direcao.y };
    jogo.ultimoPasso = jogo.direcao;

    // parede
    if (novaCabeca.x < 0 || novaCabeca.x >= COLUNAS ||
        novaCabeca.y < 0 || novaCabeca.y >= LINHAS) {
        jogo.gameOver = true;
        return;
    }

    // corpo (ignorando o rabo, que vai sair do lugar)
    for (size_t i = 0; i < jogo.cobra.size() - 1; i++) {
        if (jogo.cobra[i].x == novaCabeca.x && jogo.cobra[i].y == novaCabeca.y) {
            jogo.gameOver = true;
            return;
        }
    }

    jogo.cobra.insert(jogo.cobra.begin(), novaCabeca);

    if (novaCabeca.x == jogo.comida.x && novaCabeca.y == jogo.comida.y) {
        jogo.pontuacao += 1;

        // encheu a grade: vitória (e não há onde sortear comida)
        if ((int)jogo.cobra.size() == COLUNAS * LINHAS) {
            jogo.vitoria = true;
            jogo.gameOver = true;
            return;
        }

        jogo.comida = sortearComida(jogo.cobra);
    } else {
        jogo.cobra.pop_back();
    }
}

void desenhar(const Jogo& jogo) {
    BeginDrawing();
    ClearBackground(BLACK);

    // grade
    Color corGrade = {40, 40, 40, 255};
    for (int x = 0; x <= COLUNAS; x++) {
        DrawLine(x * TAMANHO_CELULA, 0, x * TAMANHO_CELULA, LINHAS * TAMANHO_CELULA, corGrade);
    }
    for (int y = 0; y <= LINHAS; y++) {
        DrawLine(0, y * TAMANHO_CELULA, COLUNAS * TAMANHO_CELULA, y * TAMANHO_CELULA, corGrade);
    }

    // cobra
    for (size_t i = 0; i < jogo.cobra.size(); i++) {
        Color cor = (i == 0) ? YELLOW : GREEN;
        DrawRectangle(jogo.cobra[i].x * TAMANHO_CELULA, jogo.cobra[i].y * TAMANHO_CELULA,
                      TAMANHO_CELULA, TAMANHO_CELULA, cor);
    }

    // comida (na vitória ela foi comida e não há uma nova)
    if (!jogo.vitoria) {
        DrawRectangle(jogo.comida.x * TAMANHO_CELULA, jogo.comida.y * TAMANHO_CELULA,
                      TAMANHO_CELULA, TAMANHO_CELULA, RED);
    }

    DrawText(TextFormat("Pontos: %d", jogo.pontuacao), 10, 10, 20, WHITE);

    // mensagens de fim
    if (jogo.gameOver) {
        int meioY = (LINHAS * TAMANHO_CELULA) / 2;

        if (jogo.vitoria) {
            desenharTextoCentralizado("VOCE VENCEU!", meioY - 40, 40, GOLD);
        } else {
            desenharTextoCentralizado("FIM DE JOGO!", meioY - 40, 40, RED);
        }
        desenharTextoCentralizado("Aperte R para reiniciar", meioY + 10, 20, GRAY);
    }

    EndDrawing();
}
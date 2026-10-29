#pragma once

#include <vector>

const int TAMANHO_CELULA = 45;
const int COLUNAS = 20;
const int LINHAS = 20;
const float INTERVALO = 0.125f;

struct Posicao {
    int x;
    int y;
};

struct Jogo {
    std::vector<Posicao> cobra;
    Posicao comida;
    float acumulador;
    Posicao direcao;
    Posicao ultimoPasso;
    int pontuacao;
    bool gameOver;
    bool vitoria;
};

void reiniciar(Jogo& jogo);
void lerInput(Jogo& jogo);
void atualizar(Jogo& jogo);
void desenhar(const Jogo& jogo);
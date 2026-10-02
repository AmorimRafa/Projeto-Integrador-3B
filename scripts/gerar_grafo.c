/*
 * gerar_grafo.c — Gera grafos direcionados aleatórios para testes de
 * desempenho, no formato edge list (V E na 1ª linha, "u v" depois).
 * Uso: gerar_grafo <V> <E> <semente>
 * 80% das arestas formam ciclos (CFCs); 20% são aleatórias.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "uso: %s <V> <E> <semente>\n", argv[0]);
        return 2;
    }
    int V = atoi(argv[1]);
    int E = atoi(argv[2]);
    unsigned semente = (unsigned)atol(argv[3]);
    if (V <= 1 || E <= 0) return 2;

    srand(semente);
    printf("%d %d\n", V, E);

    int em_ciclos = (int)(E * 0.8);

    /* ciclos: percorre os vértices em blocos, fechando cada bloco num ciclo */
    int u = 0;
    for (int i = 0; i < em_ciclos; i++) {
        int v = (u + 1) % V;
        printf("%d %d\n", u, v);
        u = v;
        if (rand() % 25 == 0) /* fecha bloco ~ a cada 25 arestas: novo ciclo */
            u = rand() % V;
    }

    /* arestas aleatórias restantes */
    for (int i = em_ciclos; i < E; i++)
        printf("%d %d\n", rand() % V, rand() % V);

    return 0;
}

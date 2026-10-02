#ifndef MATRIZ_ADJ_H
#define MATRIZ_ADJ_H

#include <stddef.h>

/*
 * matriz_adj.h — Matriz de adjacência para grafo direcionado.
 * M[u][v] == 1  se existe aresta u -> v, 0 caso contrário.
 */

typedef struct {
    int n;   /* quantidade de vértices */
    int **m; /* matriz n x n */
} MatrizAdj;

/* Cria matriz zerada para n vértices. Retorna NULL em erro. */
MatrizAdj *matriz_criar(int n);

/* Insere aresta u -> v (marca m[u][v] = 1). Retorna 0 em sucesso. */
int matriz_inserir(MatrizAdj *m, int u, int v);

/* Consulta se existe aresta u -> v. Retorna 1 se sim, 0 se não. */
int matriz_tem_aresta(const MatrizAdj *m, int u, int v);

/* Libera toda a memória da matriz. */
void matriz_liberar(MatrizAdj *m);

/* Estima a memória (bytes) usada pela matriz: ponteiros + células int. */
size_t matriz_memoria(const MatrizAdj *m);

#endif /* MATRIZ_ADJ_H */

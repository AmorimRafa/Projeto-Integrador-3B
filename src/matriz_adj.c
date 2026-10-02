#include "matriz_adj.h"

#include <stdlib.h>

MatrizAdj *matriz_criar(int n) {
    if (n <= 0) return NULL;
    MatrizAdj *m = malloc(sizeof(MatrizAdj));
    if (!m) return NULL;
    m->n = n;
    m->m = malloc(n * sizeof(int *));
    if (!m->m) { free(m); return NULL; }
    for (int i = 0; i < n; i++) {
        m->m[i] = calloc(n, sizeof(int));
        if (!m->m[i]) {
            for (int j = 0; j < i; j++) free(m->m[j]);
            free(m->m);
            free(m);
            return NULL;
        }
    }
    return m;
}

int matriz_inserir(MatrizAdj *m, int u, int v) {
    if (!m || u < 0 || u >= m->n || v < 0 || v >= m->n)
        return 1;
    m->m[u][v] = 1;
    return 0;
}

int matriz_tem_aresta(const MatrizAdj *m, int u, int v) {
    if (!m || u < 0 || u >= m->n || v < 0 || v >= m->n)
        return 0;
    return m->m[u][v];
}

void matriz_liberar(MatrizAdj *m) {
    if (!m) return;
    for (int i = 0; i < m->n; i++)
        free(m->m[i]);
    free(m->m);
    free(m);
}

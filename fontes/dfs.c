#include "dfs.h"

#include <stdlib.h>

void dfs_ordem(const ListaAdj *l, int v, int *visitado, int *ordem, int *k) {
    visitado[v] = 1;
    ordem[(*k)++] = v;
    for (const NoViz *p = lista_vizinhos(l, v); p; p = p->prox)
        if (!visitado[p->v])
            dfs_ordem(l, p->v, visitado, ordem, k);
}

static void dfs_pos(const ListaAdj *l, int v, int *visitado, int *pos, int *k) {
    visitado[v] = 1;
    for (const NoViz *p = lista_vizinhos(l, v); p; p = p->prox)
        if (!visitado[p->v])
            dfs_pos(l, p->v, visitado, pos, k);
    pos[(*k)++] = v; /* pós-ordem: v só após todos os sucessores */
}

int dfs_completa(const ListaAdj *l, int *visitado, int *ordem_saida) {
    if (!l || !visitado || !ordem_saida) return 1;
    int *pos = malloc(l->n * sizeof(int));
    if (!pos) return 1;
    for (int i = 0; i < l->n; i++) visitado[i] = 0;
    int k = 0;
    for (int i = 0; i < l->n; i++)
        if (!visitado[i])
            dfs_pos(l, i, visitado, pos, &k);
    /* ordem de finalização decrescente = reverso da pós-ordem */
    for (int i = 0; i < l->n; i++)
        ordem_saida[i] = pos[l->n - 1 - i];
    free(pos);
    return 0;
}

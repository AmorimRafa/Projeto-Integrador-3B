#include "scc.h"

#include <stdlib.h>

typedef struct {
    const ListaAdj *g;
    int n;
    int *idx;      /* índice de descoberta */
    int *low;      /* low-link */
    int *na_pilha; /* marca de presença na pilha */
    int *pilha;
    int topo;
    int tempo;
    int *comp;     /* saída: componente por vértice */
    int n_comp;
} TarjanCtx;

static void tarjan_dfs(TarjanCtx *c, int v) {
    c->idx[v] = c->low[v] = c->tempo++;
    c->pilha[c->topo++] = v;
    c->na_pilha[v] = 1;

    for (const NoViz *p = lista_vizinhos(c->g, v); p; p = p->prox) {
        int w = p->v;
        if (c->idx[w] < 0) {
            tarjan_dfs(c, w);
            if (c->low[w] < c->low[v]) c->low[v] = c->low[w];
        } else if (c->na_pilha[w]) {
            if (c->idx[w] < c->low[v]) c->low[v] = c->idx[w];
        }
    }

    if (c->low[v] == c->idx[v]) {
        /* v é raiz de uma CFC: desempilha até v */
        int w;
        do {
            w = c->pilha[--c->topo];
            c->na_pilha[w] = 0;
            c->comp[w] = c->n_comp;
        } while (w != v);
        c->n_comp++;
    }
}

SCC *scc_tarjan(const ListaAdj *l) {
    if (!l) return NULL;
    TarjanCtx c;
    c.g = l; c.n = l->n; c.n_comp = 0; c.tempo = 0; c.topo = 0;
    c.idx = malloc(l->n * sizeof(int));
    c.low = malloc(l->n * sizeof(int));
    c.na_pilha = calloc(l->n, sizeof(int));
    c.pilha = malloc(l->n * sizeof(int));
    c.comp = malloc(l->n * sizeof(int));
    if (!c.idx || !c.low || !c.na_pilha || !c.pilha || !c.comp) {
        free(c.idx); free(c.low); free(c.na_pilha); free(c.pilha); free(c.comp);
        return NULL;
    }
    for (int i = 0; i < l->n; i++) c.idx[i] = -1;

    for (int i = 0; i < l->n; i++)
        if (c.idx[i] < 0)
            tarjan_dfs(&c, i);

    SCC *s = malloc(sizeof(SCC));
    if (!s) { free(c.idx); free(c.low); free(c.na_pilha); free(c.pilha); free(c.comp); return NULL; }
    s->n = l->n;
    s->n_comp = c.n_comp;
    s->comp = c.comp;
    s->tam = calloc(c.n_comp, sizeof(int));
    if (!s->tam) { free(s->comp); free(s); s = NULL; }
    else
        for (int i = 0; i < l->n; i++)
            s->tam[s->comp[i]]++;

    free(c.idx); free(c.low); free(c.na_pilha); free(c.pilha);
    return s;
}

void scc_liberar(SCC *s) {
    if (!s) return;
    free(s->comp);
    free(s->tam);
    free(s);
}

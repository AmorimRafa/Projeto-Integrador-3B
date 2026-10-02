#include "armadilha.h"

#include <stdlib.h>

AnaliseArmadilhas *armadilha_analisar(const ListaAdj *l, const SCC *s) {
    if (!l || !s) return NULL;

    AnaliseArmadilhas *a = malloc(sizeof(AnaliseArmadilhas));
    if (!a) return NULL;
    a->n_comp = s->n_comp;
    a->terminal = calloc(s->n_comp, sizeof(int));
    a->n_terminais = 0;
    a->n_tamanho1_sem_laco = 0;
    a->n_tamanho1_com_laco = 0;
    a->n_tamanho_maior1 = 0;
    if (!a->terminal) { free(a); return NULL; }

    /* todo componente começa como "terminal"; se acharmos aresta saindo,
     * marcamos como não-terminal */
    for (int c = 0; c < s->n_comp; c++)
        a->terminal[c] = 1;

    /* percorre todas as arestas u -> v */
    for (int u = 0; u < l->n; u++) {
        for (const NoViz *p = lista_vizinhos(l, u); p; p = p->prox) {
            int cu = s->comp[u];
            int cv = s->comp[p->v];
            if (cu != cv)
                a->terminal[cu] = 0; /* CFC de u tem saída */
        }
    }

    /* auto-laço por componente unitário */
    for (int c = 0; c < s->n_comp; c++) {
        if (s->tam[c] > 1) {
            a->n_tamanho_maior1++;
            continue;
        }
        /* acha o único vértice da CFC c */
        int u = -1;
        for (int i = 0; i < l->n && u < 0; i++)
            if (s->comp[i] == c) u = i;
        int tem_laco = 0;
        for (const NoViz *p = lista_vizinhos(l, u); p; p = p->prox)
            if (p->v == u) tem_laco = 1;
        if (tem_laco) a->n_tamanho1_com_laco++;
        else          a->n_tamanho1_sem_laco++;
    }

    for (int c = 0; c < s->n_comp; c++)
        if (a->terminal[c])
            a->n_terminais++;

    return a;
}

void armadilha_liberar(AnaliseArmadilhas *a) {
    if (!a) return;
    free(a->terminal);
    free(a);
}

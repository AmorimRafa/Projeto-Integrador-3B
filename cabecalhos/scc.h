#ifndef SCC_H
#define SCC_H

#include "lista_adj.h"

/*
 * scc.h — Componentes Fortemente Conexas via algoritmo de Tarjan.
 */

typedef struct {
    int n;        /* vértices */
    int n_comp;   /* quantidade de CFCs */
    int *comp;    /* comp[v] = id da CFC de v, em [0, n_comp) */
    int *tam;     /* tam[c] = tamanho da CFC c */
} SCC;

/* Executa Tarjan sobre l. Retorna NULL em erro de alocação. */
SCC *scc_tarjan(const ListaAdj *l);

void scc_liberar(SCC *s);

#endif /* SCC_H */

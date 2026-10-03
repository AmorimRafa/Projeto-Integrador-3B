#ifndef DFS_H
#define DFS_H

#include "lista_adj.h"

/* DFS a partir de v; marca visitado[] e preenche ordem[] com a ordem de
 * visita. *k é o contador de vértices visitados. */
void dfs_ordem(const ListaAdj *l, int v, int *visitado, int *ordem, int *k);

/* DFS do grafo inteiro: preenche ordem de finalização (pós-ordem reversa)
 * em *ordem_saida[0..n-1]. Retorna 0 em sucesso. */
int dfs_completa(const ListaAdj *l, int *visitado, int *ordem_saida);

#endif /* DFS_H */

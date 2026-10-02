#include "graph.h"
#include "lista_adj.h"
#include "matriz_adj.h"

#include <stdlib.h>

int grafo_iniciar(Grafo *g, int n_vertices, Representacao repr) {
    if (!g || n_vertices <= 0) return 1;
    g->n_vertices = n_vertices;
    g->n_arestas = 0;
    g->repr = repr;
    if (repr == REPR_LISTA)
        g->dados = lista_criar(n_vertices);
    else
        g->dados = matriz_criar(n_vertices);
    return g->dados ? 0 : 1;
}

int grafo_adicionar_aresta(Grafo *g, int u, int v) {
    if (!g || !g->dados) return 1;
    int rc;
    if (g->repr == REPR_LISTA)
        rc = lista_inserir((ListaAdj *)g->dados, u, v);
    else
        rc = matriz_inserir((MatrizAdj *)g->dados, u, v);
    if (rc == 0)
        g->n_arestas++;
    return rc;
}

void grafo_liberar(Grafo *g) {
    if (!g || !g->dados) return;
    if (g->repr == REPR_LISTA)
        lista_liberar((ListaAdj *)g->dados);
    else
        matriz_liberar((MatrizAdj *)g->dados);
    g->dados = NULL;
}

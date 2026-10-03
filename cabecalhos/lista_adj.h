#ifndef LISTA_ADJ_H
#define LISTA_ADJ_H

#include <stddef.h>

/*
 * lista_adj.h — Lista de adjacência para grafo direcionado.
 * Cada vértice u tem uma lista encadeada dos seus sucessores v (u -> v).
 */

typedef struct NoViz
{
    int v; /* vértice destino */
    struct NoViz *prox;
} NoViz;

typedef struct
{
    int n;       /* quantidade de vértices */
    NoViz **adj; /* array adj[u] = cabeça da lista de sucessores de u */
} ListaAdj;

/* Cria lista vazia para n vértices. Retorna NULL em erro de alocação. */
ListaAdj *lista_criar(int n);

/* Insere aresta u -> v. Retorna 0 em sucesso. */
int lista_inserir(ListaAdj *l, int u, int v);

/* Retorna a cabeça da lista de sucessores de u (pode ser NULL). */
const NoViz *lista_vizinhos(const ListaAdj *l, int u);

/* Libera toda a memória da lista. */
void lista_liberar(ListaAdj *l);

/* Estima a memória (bytes) usada pela lista: cabeças + nós das arestas. */
size_t lista_memoria(const ListaAdj *l);

#endif /* LISTA_ADJ_H */

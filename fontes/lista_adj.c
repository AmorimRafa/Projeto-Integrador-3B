#include "lista_adj.h"

#include <stdlib.h>

size_t lista_memoria(const ListaAdj *l)
{
    if (!l)
        return 0;
    size_t total = sizeof(ListaAdj) + l->n * sizeof(NoViz *);
    for (int i = 0; i < l->n; i++)
        for (const NoViz *p = l->adj[i]; p; p = p->prox)
            total += sizeof(NoViz);
    return total;
}

ListaAdj *lista_criar(int n)
{
    if (n <= 0)
        return NULL;
    ListaAdj *l = malloc(sizeof(ListaAdj));
    if (!l)
        return NULL;
    l->n = n;
    l->adj = calloc(n, sizeof(NoViz *));
    if (!l->adj)
    {
        free(l);
        return NULL;
    }
    return l;
}

int lista_inserir(ListaAdj *l, int u, int v)
{
    if (!l || u < 0 || u >= l->n || v < 0 || v >= l->n)
        return 1;
    NoViz *no = malloc(sizeof(NoViz));
    if (!no)
        return 1;
    no->v = v;
    no->prox = l->adj[u];
    l->adj[u] = no;
    return 0;
}

const NoViz *lista_vizinhos(const ListaAdj *l, int u)
{
    if (!l || u < 0 || u >= l->n)
        return NULL;
    return l->adj[u];
}

void lista_liberar(ListaAdj *l)
{
    if (!l)
        return;
    for (int i = 0; i < l->n; i++)
    {
        NoViz *p = l->adj[i];
        while (p)
        {
            NoViz *tmp = p;
            p = p->prox;
            free(tmp);
        }
    }
    free(l->adj);
    free(l);
}

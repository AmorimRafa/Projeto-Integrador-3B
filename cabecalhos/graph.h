#ifndef GRAPH_H
#define GRAPH_H

/*
 * graph.h — Modelo de grafo direcionado da malha viária.
 *
 * G = (V, E):
 *   V: vértices = interseções, identificadas por 0 .. n-1
 *   E: arestas direcionadas (trechos de rua) u -> v
 *
 * Fase I: grafo não ponderado (pesos entram na Fase II).
 */

typedef enum {
    REPR_LISTA = 0,
    REPR_MATRIZ = 1
} Representacao;

typedef struct {
    int n_vertices;     /* quantidade de vértices |V| */
    int n_arestas;      /* quantidade de arestas |E|  */
    Representacao repr; /* lista ou matriz de adjacência */
    void *dados;        /* ponteiro para a estrutura concreta (ListaAdj* ou MatrizAdj*) */
} Grafo;

/* Inicializa um grafo vazio com a representação escolhida. */
int grafo_iniciar(Grafo *g, int n_vertices, Representacao repr);

/* Insere aresta direcionada u -> v. Retorna 0 em sucesso, != 0 em erro. */
int grafo_adicionar_aresta(Grafo *g, int u, int v);

/* Libera toda a memória associada ao grafo. */
void grafo_liberar(Grafo *g);

#endif /* GRAPH_H */

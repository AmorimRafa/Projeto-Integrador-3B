/*
 * test_cfc.c — Valida lista vs matriz e testa Tarjan em grafos pequenos.
 * Uso: test_cfc <arquivo.txt> <n_comp_esperado>
 */
#include <stdio.h>
#include <stdlib.h>

#include "io.h"
#include "lista_adj.h"
#include "matriz_adj.h"
#include "scc.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "uso: %s <arquivo.txt> <n_comp_esperado>\n", argv[0]);
        return 2;
    }
    int esperado = atoi(argv[2]);

    int n, m, *U, *V;
    if (io_ler_arestas(argv[1], &n, &m, &U, &V) != 0) return 2;
    if (io_validar_vertices(U, V, m, n) != 0) return 2;

    int loops, dups;
    io_processar_arestas(U, V, m, &loops, &dups);
    printf("V=%d E=%d auto-lacos=%d duplicadas=%d\n", n, m, loops, dups);

    ListaAdj *l = lista_criar(n);
    MatrizAdj *mz = matriz_criar(n);
    for (int i = 0; i < m; i++) {
        lista_inserir(l, U[i], V[i]);
        matriz_inserir(mz, U[i], V[i]);
    }

    /* valida lista vs matriz */
    for (int u = 0; u < n; u++) {
        for (const NoViz *p = lista_vizinhos(l, u); p; p = p->prox)
            if (!matriz_tem_aresta(mz, u, p->v)) {
                printf("ERRO: aresta %d->%d na lista, ausente na matriz\n", u, p->v);
                return 1;
            }
    }
    for (int u = 0; u < n; u++)
        for (int v = 0; v < n; v++)
            if (matriz_tem_aresta(mz, u, v)) {
                int achou = 0;
                for (const NoViz *p = lista_vizinhos(l, u); p; p = p->prox)
                    if (p->v == v) achou = 1;
                if (!achou) {
                    printf("ERRO: aresta %d->%d na matriz, ausente na lista\n", u, v);
                    return 1;
                }
            }
    printf("OK: lista e matriz consistentes\n");

    SCC *s = scc_tarjan(l);
    if (!s) { printf("ERRO: tarjan falhou\n"); return 1; }
    printf("Tarjan: %d CFCs (esperado %d)\n", s->n_comp, esperado);
    for (int c = 0; c < s->n_comp; c++)
        printf("  CFC %d: tamanho %d\n", c, s->tam[c]);

    int ok = (s->n_comp == esperado);
    printf(ok ? "PASS\n" : "FAIL\n");

    scc_liberar(s);
    lista_liberar(l);
    matriz_liberar(mz);
    free(U); free(V);
    return ok ? 0 : 1;
}

/*
 * main.c — Ponto de entrada do programa da Fase I.
 * Uso: grafo <arquivo.txt> [--matriz]
 * Carrega malha viária, roda Tarjan (CFCs) e análise de armadilhas.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "lista_adj.h"
#include "matriz_adj.h"
#include "io.h"
#include "scc.h"
#include "armadilha.h"
#include "metricas.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "uso: %s <arquivo.txt> [--matriz]\n", argv[0]);
        return 2;
    }
    Representacao repr = REPR_LISTA;
    for (int i = 2; i < argc; i++)
        if (strcmp(argv[i], "--matriz") == 0)
            repr = REPR_MATRIZ;

    Cronometro t_leitura, t_tarjan, t_armadilha;

    cronometro_iniciar(&t_leitura);
    int n, m, loops, dups, *U, *V;
    if (io_ler_arestas(argv[1], &n, &m, &U, &V) != 0) return 2;
    if (io_validar_vertices(U, V, m, n) != 0) { free(U); free(V); return 2; }
    io_processar_arestas(U, V, m, &loops, &dups);
    printf("Leitura+validacao: %.2f ms\n", cronometro_ms(&t_leitura));
    printf("V=%d E=%d auto-lacos=%d duplicadas=%d\n", n, m, loops, dups);

    Grafo g;
    if (grafo_init(&g, n, repr) != 0) { free(U); free(V); return 1; }
    for (int i = 0; i < m; i++)
        grafo_add_aresta(&g, U[i], V[i]);

    /* CFC e armadilhas exigem a lista de adjacência */
    ListaAdj *l = (repr == REPR_LISTA) ? (ListaAdj *)g.dados : NULL;
    ListaAdj *l_tmp = NULL;
    if (repr == REPR_MATRIZ) {
        l_tmp = lista_criar(n);
        MatrizAdj *mz = (MatrizAdj *)g.dados;
        for (int u = 0; u < n; u++)
            for (int v = 0; v < n; v++)
                if (matriz_tem_aresta(mz, u, v))
                    lista_inserir(l_tmp, u, v);
        l = l_tmp;
    }

    cronometro_iniciar(&t_tarjan);
    SCC *s = scc_tarjan(l);
    if (!s) { fprintf(stderr, "erro no tarjan\n"); return 1; }
    printf("\nTarjan: %.2f ms\n", cronometro_ms(&t_tarjan));
    printf("CFCs encontradas: %d\n", s->n_comp);
    for (int c = 0; c < s->n_comp; c++)
        printf("  CFC %d: tamanho %d\n", c, s->tam[c]);

    cronometro_iniciar(&t_armadilha);
    AnaliseArmadilhas *a = armadilha_analisar(l, s);
    printf("Analise de armadilhas: %.2f ms\n", cronometro_ms(&t_armadilha));
    if (a) {
        printf("\nArmadilhas de transito (CFCs terminais): %d\n", a->n_terminais);
        for (int c = 0; c < a->n_comp; c++)
            if (a->terminal[c])
                printf("  -> CFC %d (tamanho %d)\n", c, s->tam[c]);
        printf("CFCs unitarias sem laco: %d | com laco: %d | com >1 vertice: %d\n",
               a->n_tamanho1_sem_laco, a->n_tamanho1_com_laco, a->n_tamanho_maior1);
        armadilha_liberar(a);
    }

    scc_liberar(s);
    if (l_tmp) lista_liberar(l_tmp);
    grafo_free(&g);
    free(U); free(V);
    return 0;
}

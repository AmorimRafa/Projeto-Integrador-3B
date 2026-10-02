/*
 * test_armadilha.c — Verifica a análise de armadilhas no grafo toy.
 * Uso: test_armadilha <arquivo.txt>
 * Esperado (cfc_simples.txt): 3 CFCs, 1 terminal ({4}), 1 unitária sem
 * laço ({3}), 1 unitária com laço ({4}), 1 com tamanho > 1 ({0,1,2}).
 */
#include <stdio.h>
#include <stdlib.h>

#include "io.h"
#include "lista_adj.h"
#include "scc.h"
#include "armadilha.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "uso: %s <arquivo.txt>\n", argv[0]);
        return 2;
    }
    int n, m, *U, *V;
    if (io_ler_arestas(argv[1], &n, &m, &U, &V) != 0) return 2;

    ListaAdj *l = lista_criar(n);
    for (int i = 0; i < m; i++) lista_inserir(l, U[i], V[i]);

    SCC *s = scc_tarjan(l);
    AnaliseArmadilhas *a = armadilha_analisar(l, s);
    if (!a) { printf("ERRO\n"); return 1; }

    printf("CFCs: %d | terminais: %d | tam1 sem laco: %d | tam1 com laco: %d | >1: %d\n",
           a->n_comp, a->n_terminais, a->n_tamanho1_sem_laco,
           a->n_tamanho1_com_laco, a->n_tamanho_maior1);

    int ok = (a->n_comp == 3 && a->n_terminais == 1 &&
              a->n_tamanho1_sem_laco == 1 && a->n_tamanho1_com_laco == 1 &&
              a->n_tamanho_maior1 == 1);
    printf(ok ? "PASS\n" : "FAIL\n");

    armadilha_liberar(a);
    scc_liberar(s);
    lista_liberar(l);
    free(U); free(V);
    return ok ? 0 : 1;
}

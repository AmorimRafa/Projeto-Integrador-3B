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
#include "registro.h"


int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "uso: %s <arquivo.txt> [--matriz]\n", argv[0]);
        return 2;
    }
    Representacao repr = REPR_LISTA;
    int comparar = 0;
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "--matriz") == 0)
            repr = REPR_MATRIZ;
        else if (strcmp(argv[i], "--comparar") == 0)
            comparar = 1;
    }

    Cronometro t_leitura, t_tarjan, t_armadilha;

    cronometro_iniciar(&t_leitura);
    int n, m, loops, dups, *U, *V;
    if (io_ler_arestas(argv[1], &n, &m, &U, &V) != 0) return 2;
    if (io_validar_vertices(U, V, m, n) != 0) { free(U); free(V); return 2; }
    io_processar_arestas(U, V, m, &loops, &dups);
    double ms_leitura = cronometro_ms(&t_leitura);
    printf("Leitura+validacao: %.2f ms\n", ms_leitura);
    printf("V=%d E=%d auto-lacos=%d duplicadas=%d\n", n, m, loops, dups);

    Grafo g;
    if (grafo_iniciar(&g, n, repr) != 0) { free(U); free(V); return 1; }
    for (int i = 0; i < m; i++)
        grafo_adicionar_aresta(&g, U[i], V[i]);

    /* mede a memória da estrutura construída pelo Grafo */
    if (repr == REPR_LISTA)
        printf("Memoria lista: %zu bytes\n", lista_memoria((ListaAdj *)g.dados));
    else
        printf("Memoria matriz: %zu bytes\n", matriz_memoria((MatrizAdj *)g.dados));

    /* modo comparação: constrói as duas e imprime memória lado a lado */
    if (comparar) {
        ListaAdj *lcmp = lista_criar(n);
        MatrizAdj *mcmp = matriz_criar(n);
        for (int i = 0; i < m; i++) {
            lista_inserir(lcmp, U[i], V[i]);
            matriz_inserir(mcmp, U[i], V[i]);
        }
        printf("\n--- Comparacao lista x matriz ---\n");
        printf("Memoria lista : %zu bytes\n", lista_memoria(lcmp));
        printf("Memoria matriz: %zu bytes\n", matriz_memoria(mcmp));
        lista_liberar(lcmp);
        matriz_liberar(mcmp);
    }

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
    double ms_tarjan = cronometro_ms(&t_tarjan);
    printf("\nTarjan: %.2f ms\n", ms_tarjan);
    printf("CFCs encontradas: %d\n", s->n_comp);
    int limite = s->n_comp < 10 ? s->n_comp : 10;
    for (int c = 0; c < limite; c++)
        printf("  CFC %d: tamanho %d\n", c, s->tam[c]);
    if (s->n_comp > 10) {
        int maior = 0;
        for (int c = 0; c < s->n_comp; c++)
            if (s->tam[c] > maior) maior = s->tam[c];
        printf("  ... (%d CFCs omitidas; maior CFC tem %d vertices)\n",
               s->n_comp - 10, maior);
    }

    cronometro_iniciar(&t_armadilha);
    AnaliseArmadilhas *a = armadilha_analisar(l, s);
    double ms_arm = cronometro_ms(&t_armadilha);
    printf("Analise de armadilhas: %.2f ms\n", ms_arm);
    if (a) {
        printf("\nArmadilhas de transito (CFCs terminais): %d\n", a->n_terminais);
        int mostradas = 0;
        for (int c = 0; c < a->n_comp && mostradas < 20; c++)
            if (a->terminal[c]) {
                printf("  -> CFC %d (tamanho %d)\n", c, s->tam[c]);
                mostradas++;
            }
        if (a->n_terminais > 20)
            printf("  ... (%d armadilhas omitidas)\n", a->n_terminais - 20);
        printf("CFCs unitarias sem laco: %d | com laco: %d | com >1 vertice: %d\n",
               a->n_tamanho1_sem_laco, a->n_tamanho1_com_laco, a->n_tamanho_maior1);

        Resultado r;
        r.dataset = argv[1];
        r.repr = (repr == REPR_LISTA) ? "lista" : "matriz";
        r.n_vertices = n;
        r.n_arestas = m;
        r.n_cfc = s->n_comp;
        r.n_armadilhas = a->n_terminais;
        r.ms_leitura = ms_leitura;
        r.ms_tarjan = ms_tarjan;
        r.ms_armadilha = ms_arm;
        resultado_registrar("resultados/resultados.csv", &r);
        printf("Resultado registrado em resultados/resultados.csv\n");
        armadilha_liberar(a);
    }

    scc_liberar(s);
    if (l_tmp) lista_liberar(l_tmp);
    grafo_liberar(&g);
    free(U); free(V);
    return 0;
}

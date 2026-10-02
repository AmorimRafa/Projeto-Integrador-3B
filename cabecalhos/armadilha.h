#ifndef ARMADILHA_H
#define ARMADILHA_H

#include "lista_adj.h"
#include "scc.h"

/*
 * armadilha.h — Análise de "armadilhas de trânsito" a partir das CFCs.
 *
 * Uma CFC é classificada como:
 *  - terminal: nenhuma aresta sai dela para outra CFC (no DAG de
 *    condensação). Carro entra e não consegue sair/voltar -> armadilha.
 *  - com saída: possui pelo menos uma aresta para outra CFC.
 * Também conta CFCs de tamanho 1 (com/sem auto-laço) e de tamanho > 1.
 */

typedef struct {
    int n_comp;              /* total de CFCs */
    int *terminal;           /* terminal[c] = 1 se a CFC c não tem saída */
    int n_terminais;         /* quantidade de CFCs terminais */
    int n_tamanho1_sem_laco; /* CFCs unitárias sem auto-laço */
    int n_tamanho1_com_laco; /* CFCs unitárias com auto-laço */
    int n_tamanho_maior1;    /* CFCs com 2+ vértices */
} AnaliseArmadilhas;

/* Analisa o grafo e as CFCs. Retorna NULL em erro de alocação. */
AnaliseArmadilhas *armadilha_analisar(const ListaAdj *l, const SCC *s);

void armadilha_liberar(AnaliseArmadilhas *a);

#endif /* ARMADILHA_H */

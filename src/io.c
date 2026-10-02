#include "io.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Pula linhas em branco e comentários (#). Retorna a linha lida ou NULL. */
static char *proxima_linha_util(FILE *f, char *buf, int tam) {
    while (fgets(buf, tam, f) != NULL) {
        char *p = buf;
        while (*p == ' ' || *p == '\t') p++;
        if (*p == '\n' || *p == '\0' || *p == '#')
            continue;
        return buf;
    }
    return NULL;
}

int io_ler_header(const char *caminho, int *n_vertices, int *n_arestas) {
    FILE *f = fopen(caminho, "r");
    if (!f) {
        fprintf(stderr, "io: nao foi possivel abrir %s\n", caminho);
        return 1;
    }
    char buf[256];
    if (!proxima_linha_util(f, buf, sizeof(buf)) ||
        sscanf(buf, "%d %d", n_vertices, n_arestas) != 2) {
        fprintf(stderr, "io: cabecalho invalido (esperado \"V E\")\n");
        fclose(f);
        return 1;
    }
    if (*n_vertices <= 0 || *n_arestas < 0) {
        fprintf(stderr, "io: cabecalho com valores invalidos\n");
        fclose(f);
        return 1;
    }
    fclose(f);
    return 0;
}

int io_ler_arestas(const char *caminho, int *n_vertices, int *n_arestas,
                   int **U, int **V) {
    if (io_ler_header(caminho, n_vertices, n_arestas) != 0)
        return 1;

    FILE *f = fopen(caminho, "r");
    if (!f) return 1;

    int *u = malloc((*n_arestas) * sizeof(int));
    int *v = malloc((*n_arestas) * sizeof(int));
    if (!u || !v) { free(u); free(v); fclose(f); return 1; }

    char buf[256];
    /* pula o cabeçalho */
    proxima_linha_util(f, buf, sizeof(buf));

    for (int i = 0; i < *n_arestas; i++) {
        if (!proxima_linha_util(f, buf, sizeof(buf)) ||
            sscanf(buf, "%d %d", &u[i], &v[i]) != 2) {
            fprintf(stderr, "io: aresta %d invalida\n", i);
            free(u); free(v); fclose(f);
            return 1;
        }
    }
    fclose(f);
    *U = u;
    *V = v;
    return 0;
}

int io_validar_vertices(const int *U, const int *V, int n_arestas, int n_vertices) {
    for (int i = 0; i < n_arestas; i++) {
        if (U[i] < 0 || U[i] >= n_vertices || V[i] < 0 || V[i] >= n_vertices) {
            fprintf(stderr, "io: vertice fora de [0,%d) na aresta %d: %d -> %d\n",
                    n_vertices, i, U[i], V[i]);
            return 1;
        }
    }
    return 0;
}

int io_processar_arestas(const int *U, const int *V, int n_arestas,
                         int *auto_lacos, int *duplicadas) {
    *auto_lacos = 0;
    *duplicadas = 0;
    for (int i = 0; i < n_arestas; i++) {
        if (U[i] == V[i])
            (*auto_lacos)++;
        for (int j = 0; j < i; j++)
            if (U[i] == U[j] && V[i] == V[j])
                (*duplicadas)++;
    }
    return 0;
}

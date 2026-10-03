#ifndef REGISTRO_H
#define REGISTRO_H

#include <stddef.h>

/*
 * registro.h — Registro dos resultados da execução em arquivo CSV.
 */

typedef struct {
    const char *dataset;
    const char *repr;
    int n_vertices;
    int n_arestas;
    int n_cfc;
    int n_armadilhas;

    size_t memoria_bytes;

    double ms_leitura;
    double ms_tarjan;
    double ms_armadilha;
} Resultado;

/* Acrescenta uma linha no CSV. */
int resultado_registrar(const char *caminho_csv, const Resultado *r);

#endif
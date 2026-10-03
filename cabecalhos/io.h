#ifndef IO_H
#define IO_H
// Header correpondente ao io.c
/*
 * io.h — Leitura e processamento do arquivo de entrada (edge list).
 * Formato: "V E" na 1ª linha, depois "u v" por aresta (ver docs/formato_entrada.md).
 */

/* Lê o cabeçalho "V E". Retorna 0 em sucesso. */
int io_ler_header(const char *caminho, int *n_vertices, int *n_arestas);

/* Lê todas as arestas para arrays alocados dinamicamente (*U e *V devem
 * ser liberados com free pelo chamador). Retorna 0 em sucesso. */
int io_ler_arestas(const char *caminho, int *n_vertices, int *n_arestas,
                   int **U, int **V);

/* Valida que todos os endpoints estão em [0, n_vertices). Retorna 0 se ok. */
int io_validar_vertices(const int *U, const int *V, int n_arestas, int n_vertices);

/* Processa as arestas: conta auto-laços e duplicadas (par origem,destino).
 * duplicadas: quantos pares se repetem. Retorna 0 em sucesso. */
int io_processar_arestas(const int *U, const int *V, int n_arestas,
                         int *auto_lacos, int *duplicadas);

#endif /* IO_H */

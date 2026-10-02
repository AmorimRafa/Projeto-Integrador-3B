#ifndef REGISTRO_H
#define REGISTRO_H

/*
 * registro.h — Registro dos resultados da execução em arquivo CSV.
 */

typedef struct
{
    const char *dataset; /* caminho/nome do arquivo de entrada */
    const char *repr;    /* "lista" ou "matriz" */
    int n_vertices;
    int n_arestas;
    int n_cfc;
    int n_armadilhas; /* CFCs terminais */
    double ms_leitura;
    double ms_tarjan;
    double ms_armadilha;
} Resultado;

/* Acrescenta uma linha no CSV (cria com cabeçalho se o arquivo não existir).
 * Retorna 0 em sucesso. */
int resultado_registrar(const char *caminho_csv, const Resultado *r);

#endif /* REGISTRO_H */

#include "registro.h"

#include <stdio.h>

int resultado_registrar(const char *caminho_csv, const Resultado *r)
{
    if (!caminho_csv || !r)
        return 1;

    /* verifica se o arquivo já existe para escrever o cabeçalho só uma vez */
    FILE *teste = fopen(caminho_csv, "r");
    int novo = (teste == NULL);
    if (teste)
        fclose(teste);

    FILE *f = fopen(caminho_csv, "a");
    if (!f)
    {
        fprintf(stderr, "registro: nao foi possivel abrir %s\n", caminho_csv);
        return 1;
    }
    if (novo)
        fprintf(f, "dataset,repr,V,E,n_cfc,n_armadilhas,ms_leitura,ms_tarjan,ms_armadilha\n");
    fprintf(f, "%s,%s,%d,%d,%d,%d,%.3f,%.3f,%.3f\n",
            r->dataset, r->repr, r->n_vertices, r->n_arestas,
            r->n_cfc, r->n_armadilhas,
            r->ms_leitura, r->ms_tarjan, r->ms_armadilha);
    fclose(f);
    return 0;
}

/*
 * gerar_graficos.c — Gera gráfico SVG (tempo de Tarjan vs V) a partir de
 * resultados/resultados.csv. Sem dependências externas.
 * Uso: gerar_graficos resultados/resultados.csv resultados/graficos/tarjan.svg
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 64

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "uso: %s <resultados.csv> <saida.svg>\n", argv[0]);
        return 2;
    }
    FILE *f = fopen(argv[1], "r");
    if (!f) { fprintf(stderr, "csv nao encontrado\n"); return 1; }

    int V[MAX]; double tarjan_lista[MAX], tarjan_matriz[MAX];
    int n = 0;

    char linha[512];
    fgets(linha, sizeof(linha), f); /* pula cabeçalho */
    while (fgets(linha, sizeof(linha), f) && n < MAX) {
        char dataset[128], repr[16];
        int v_, e_, nc, na; double l, t, a;
        if (sscanf(linha, "%127[^,],%15[^,],%d,%d,%d,%d,%lf,%lf,%lf",
                   dataset, repr, &v_, &e_, &nc, &na, &l, &t, &a) != 9)
            continue;
        if (strcmp(repr, "lista") == 0) {
            V[n] = v_; tarjan_lista[n] = t; n++;
        }
        /* assume ordem: lista depois matriz do mesmo teste */
        if (strcmp(repr, "matriz") == 0 && n > 0)
            tarjan_matriz[n - 1] = t;
    }
    fclose(f);
    if (n == 0) { fprintf(stderr, "sem dados\n"); return 1; }

    double vmax = V[n-1], tmax = 0;
    for (int i = 0; i < n; i++) {
        if (tarjan_lista[i] > tmax) tmax = tarjan_lista[i];
        if (tarjan_matriz[i] > tmax) tmax = tarjan_matriz[i];
    }
    if (tmax <= 0) tmax = 1;

    FILE *out = fopen(argv[2], "w");
    if (!out) { fprintf(stderr, "erro ao criar %s\n", argv[2]); return 1; }

    int W = 800, H = 500, M = 60;
    fprintf(out, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"%d\" height=\"%d\">\n", W, H);
    fprintf(out, "<rect width=\"100%%\" height=\"100%%\" fill=\"white\"/>\n");
    fprintf(out, "<text x=\"%d\" y=\"30\" font-size=\"18\">Tarjan: tempo (ms) vs V</text>\n", M);
    fprintf(out, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke=\"black\"/>\n", M, H-M, W-M, H-M);
    fprintf(out, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke=\"black\"/>\n", M, H-M, M, M);

    /* linha lista (azul) */
    fprintf(out, "<polyline fill=\"none\" stroke=\"blue\" stroke-width=\"2\" points=\"");
    for (int i = 0; i < n; i++) {
        double x = M + (V[i] / vmax) * (W - 2*M);
        double y = (H - M) - (tarjan_lista[i] / tmax) * (H - 2*M);
        fprintf(out, "%.1f,%.1f ", x, y);
    }
    fprintf(out, "\"/>\n");
    /* linha matriz (vermelho) */
    fprintf(out, "<polyline fill=\"none\" stroke=\"red\" stroke-width=\"2\" points=\"");
    for (int i = 0; i < n; i++) {
        double x = M + (V[i] / vmax) * (W - 2*M);
        double y = (H - M) - (tarjan_matriz[i] / tmax) * (H - 2*M);
        fprintf(out, "%.1f,%.1f ", x, y);
    }
    fprintf(out, "\"/>\n");

    fprintf(out, "<text x=\"%d\" y=\"%d\" fill=\"blue\">-- lista</text>\n", W-150, 40);
    fprintf(out, "<text x=\"%d\" y=\"%d\" fill=\"red\">-- matriz</text>\n", W-150, 60);
    fprintf(out, "</svg>\n");
    fclose(out);
    printf("Grafico salvo em %s (%d pontos)\n", argv[2], n);
    return 0;
}

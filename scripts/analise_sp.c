/*
 * analise_sp.c — Análise complementar do dataset real (centro de SP).
 *
 * Complementa o programa principal (Tarjan + armadilhas) com métricas que
 * ele não calcula:
 *   - estrutura do grafo: junções reais, grau médio, mão dupla;
 *   - alcançabilidade das armadilhas a partir da CFC gigante;
 *   - distância dos "becos sem saída" (vértices sem aresta de saída) à
 *     borda do recorte geográfico.
 *
 * Reaproveita io, lista_adj, scc e armadilha do projeto. Lê o .osm só para
 * obter as coordenadas e o número de vias de cada nó; a numeração dos
 * vértices é a mesma de scripts/osm_para_grafo.py (ids OSM ordenados como
 * texto), então o .txt e o .osm precisam ser do mesmo extrato.
 *
 * Compilar (a partir da raiz do repositório):
 *   gcc -Wall -Wextra -std=c11 -Icabecalhos -o analise_sp scripts/analise_sp.c \
 *       fontes/io.c fontes/lista_adj.c fontes/scc.c fontes/armadilha.c -lm
 *
 * Uso:
 *   ./analise_sp dados/real/sp_centro.osm dados/real/sp_centro.txt
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io.h"
#include "lista_adj.h"
#include "scc.h"
#include "armadilha.h"

#define TAM_ID 24
#define TAM_LINHA 1024
#define METROS_POR_GRAU 111320.0

typedef struct {
    char id[TAM_ID];
    double lat, lon;
} NoOsm;

typedef struct {
    char id[TAM_ID];
} RefOsm;

typedef struct {
    NoOsm *nos;       /* todos os <node> do arquivo */
    size_t n_nos;
    RefOsm *pares;    /* um item por (nó, via de carro); sem repetição dentro da via */
    size_t n_pares;
    int n_vias;       /* vias de carro com 2+ nós */
} DadosOsm;

/* Mesmo conjunto de tipos de via de scripts/osm_para_grafo.py (VIAS_CARRO). */
static const char *VIAS_CARRO[] = {
    "motorway", "trunk", "primary", "secondary", "tertiary",
    "unclassified", "residential", "service", "road", "living_street",
    "motorway_link", "trunk_link", "primary_link", "secondary_link",
    "tertiary_link"
};

static int via_de_carro(const char *tipo) {
    for (size_t i = 0; i < sizeof(VIAS_CARRO) / sizeof(VIAS_CARRO[0]); i++)
        if (strcmp(tipo, VIAS_CARRO[i]) == 0) return 1;
    return 0;
}

/* ---------- utilitários ---------- */

static int cmp_ref(const void *a, const void *b) {
    return strcmp(((const RefOsm *)a)->id, ((const RefOsm *)b)->id);
}

static int cmp_no(const void *a, const void *b) {
    return strcmp(((const NoOsm *)a)->id, ((const NoOsm *)b)->id);
}

static int cmp_int(const void *a, const void *b) {
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}

static int cmp_double(const void *a, const void *b) {
    double x = *(const double *)a, y = *(const double *)b;
    return (x > y) - (x < y);
}

/* Ordena v (modifica o vetor) e devolve a mediana. */
static double mediana(double *v, int n) {
    qsort(v, n, sizeof(double), cmp_double);
    return (n % 2) ? v[n / 2] : (v[n / 2 - 1] + v[n / 2]) / 2.0;
}

/* Copia o valor do atributo ` nome="..."` de uma linha XML. Retorna 1 se achou. */
static int extrair_atributo(const char *linha, const char *nome, char *dest, size_t tam) {
    char padrao[32];
    snprintf(padrao, sizeof(padrao), " %s=\"", nome);
    const char *p = strstr(linha, padrao);
    if (!p) return 0;
    p += strlen(padrao);
    const char *fim = strchr(p, '"');
    if (!fim) return 0;
    size_t n = (size_t)(fim - p);
    if (n >= tam) return 0;
    memcpy(dest, p, n);
    dest[n] = '\0';
    return 1;
}

static int crescer(void **vetor, size_t *cap, size_t usado, size_t tam_item) {
    if (usado < *cap) return 0;
    size_t nova = *cap ? *cap * 2 : 1024;
    void *novo = realloc(*vetor, nova * tam_item);
    if (!novo) return 1;
    *vetor = novo;
    *cap = nova;
    return 0;
}

/* ---------- leitura do .osm (o Overpass escreve um elemento por linha) ---------- */

static int ler_osm(const char *caminho, DadosOsm *d) {
    FILE *f = fopen(caminho, "r");
    if (!f) {
        fprintf(stderr, "nao foi possivel abrir %s\n", caminho);
        return 1;
    }
    memset(d, 0, sizeof(*d));

    size_t cap_nos = 0, cap_pares = 0, cap_via = 0, n_via = 0;
    RefOsm *via = NULL;           /* nós da via que está sendo lida */
    int em_via = 0;
    char tipo[64] = "";
    char linha[TAM_LINHA];
    int rc = 0;

    while (!rc && fgets(linha, sizeof(linha), f)) {
        const char *p = linha;
        while (*p == ' ' || *p == '\t') p++;

        if (strncmp(p, "<node ", 6) == 0) {
            char lat[32], lon[32];
            if (crescer((void **)&d->nos, &cap_nos, d->n_nos, sizeof(NoOsm))) { rc = 1; break; }
            NoOsm *no = &d->nos[d->n_nos];
            if (extrair_atributo(p, "id", no->id, TAM_ID) &&
                extrair_atributo(p, "lat", lat, sizeof(lat)) &&
                extrair_atributo(p, "lon", lon, sizeof(lon))) {
                no->lat = atof(lat);
                no->lon = atof(lon);
                d->n_nos++;
            }
        } else if (strncmp(p, "<way ", 5) == 0) {
            em_via = 1;
            n_via = 0;
            tipo[0] = '\0';
        } else if (em_via && strncmp(p, "<nd ", 4) == 0) {
            if (crescer((void **)&via, &cap_via, n_via, sizeof(RefOsm))) { rc = 1; break; }
            if (extrair_atributo(p, "ref", via[n_via].id, TAM_ID)) n_via++;
        } else if (em_via && strncmp(p, "<tag ", 5) == 0) {
            char k[64];
            if (extrair_atributo(p, "k", k, sizeof(k)) && strcmp(k, "highway") == 0)
                extrair_atributo(p, "v", tipo, sizeof(tipo));
        } else if (strncmp(p, "</way>", 6) == 0) {
            em_via = 0;
            if (via_de_carro(tipo) && n_via >= 2) {
                d->n_vias++;
                qsort(via, n_via, sizeof(RefOsm), cmp_ref);
                for (size_t i = 0; i < n_via; i++) {
                    if (i > 0 && strcmp(via[i].id, via[i - 1].id) == 0) continue; /* repetido na via */
                    if (crescer((void **)&d->pares, &cap_pares, d->n_pares, sizeof(RefOsm))) { rc = 1; break; }
                    d->pares[d->n_pares++] = via[i];
                }
            }
        }
    }
    free(via);
    fclose(f);
    if (rc) fprintf(stderr, "erro de alocacao ao ler o .osm\n");
    return rc;
}

/* ---------- busca em largura (alcançabilidade) ---------- */

static int bfs(const ListaAdj *g, int origem, char *visto, int *fila) {
    memset(visto, 0, g->n);
    int ini = 0, fim = 0;
    fila[fim++] = origem;
    visto[origem] = 1;
    while (ini < fim) {
        int u = fila[ini++];
        for (const NoViz *p = lista_vizinhos(g, u); p; p = p->prox)
            if (!visto[p->v]) {
                visto[p->v] = 1;
                fila[fim++] = p->v;
            }
    }
    return fim; /* quantidade de vértices alcançados */
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "uso: %s <extrato.osm> <grafo.txt>\n", argv[0]);
        return 2;
    }

    int rc = 1;
    DadosOsm osm;
    memset(&osm, 0, sizeof(osm));
    int n = 0, m = 0, *U = NULL, *V = NULL;
    ListaAdj *l = NULL, *r = NULL;
    SCC *s = NULL;
    AnaliseArmadilhas *a = NULL;
    int *usos = NULL, *rep = NULL, *fila = NULL, *tam_multi = NULL;
    char *entra = NULL, *alc_f = NULL, *alc_r = NULL;
    double *dist = NULL, *dist_becos = NULL;

    if (ler_osm(argv[1], &osm) != 0) goto fim;
    if (io_ler_arestas(argv[2], &n, &m, &U, &V) != 0) goto fim;
    if (io_validar_vertices(U, V, m, n) != 0) goto fim;

    /* vértices = ids OSM distintos, ordenados como texto (igual ao conversor);
     * em cada grupo de pares iguais, o tamanho do grupo = nº de vias do nó. */
    qsort(osm.pares, osm.n_pares, sizeof(RefOsm), cmp_ref);
    qsort(osm.nos, osm.n_nos, sizeof(NoOsm), cmp_no);

    usos = calloc(n, sizeof(int));
    double *lat = malloc(n * sizeof(double));
    double *lon = malloc(n * sizeof(double));
    if (!usos || !lat || !lon) { fprintf(stderr, "erro de alocacao\n"); free(lat); free(lon); goto fim; }

    int n_osm = 0;
    for (size_t i = 0; i < osm.n_pares; ) {
        size_t j = i;
        while (j < osm.n_pares && strcmp(osm.pares[j].id, osm.pares[i].id) == 0) j++;
        if (n_osm < n) {
            usos[n_osm] = (int)(j - i);
            NoOsm chave;
            strcpy(chave.id, osm.pares[i].id);
            NoOsm *no = bsearch(&chave, osm.nos, osm.n_nos, sizeof(NoOsm), cmp_no);
            if (!no) {
                fprintf(stderr, "no %s sem coordenadas no .osm\n", chave.id);
                free(lat); free(lon);
                goto fim;
            }
            lat[n_osm] = no->lat;
            lon[n_osm] = no->lon;
        }
        n_osm++;
        i = j;
    }
    if (n_osm != n) {
        fprintf(stderr, "o .osm gera %d vertices e o .txt tem %d: arquivos nao correspondem\n", n_osm, n);
        free(lat); free(lon);
        goto fim;
    }

    /* grafo direto e reverso, CFCs e armadilhas (módulos do projeto) */
    l = lista_criar(n);
    r = lista_criar(n);
    if (!l || !r) { fprintf(stderr, "erro de alocacao\n"); free(lat); free(lon); goto fim; }
    for (int i = 0; i < m; i++) {
        lista_inserir(l, U[i], V[i]);
        lista_inserir(r, V[i], U[i]);
    }
    s = scc_tarjan(l);
    a = s ? armadilha_analisar(l, s) : NULL;
    if (!s || !a) { fprintf(stderr, "erro no Tarjan/armadilhas\n"); free(lat); free(lon); goto fim; }

    /* ---------- estrutura do grafo ---------- */
    int juncoes = 0;
    for (int v = 0; v < n; v++)
        if (usos[v] > 1) juncoes++;

    int grau_max = 0, sem_saida = 0, sem_entrada = 0;
    long soma_grau = 0;
    for (int v = 0; v < n; v++) {
        int gs = 0, ge = 0;
        for (const NoViz *p = lista_vizinhos(l, v); p; p = p->prox) gs++;
        for (const NoViz *p = lista_vizinhos(r, v); p; p = p->prox) ge++;
        soma_grau += gs;
        if (gs > grau_max) grau_max = gs;
        if (gs == 0) sem_saida++;
        if (ge == 0) sem_entrada++;
    }

    int loops, dups;
    io_processar_arestas(U, V, m, &loops, &dups);
    int com_reversa = 0;
    for (int i = 0; i < m; i++)
        for (const NoViz *p = lista_vizinhos(l, V[i]); p; p = p->prox)
            if (p->v == U[i]) { com_reversa++; break; }

    printf("== Estrutura do grafo ==\n");
    printf("vias de carro: %d | vertices: %d | arestas: %d\n", osm.n_vias, n, m);
    printf("nos em mais de uma via (juncoes reais): %d (%.1f%% dos vertices)\n",
           juncoes, 100.0 * juncoes / n);
    printf("grau de saida medio: %.2f | maximo: %d | sem saida: %d | sem entrada: %d\n",
           (double)soma_grau / n, grau_max, sem_saida, sem_entrada);
    printf("arestas com reversa (mao dupla): %d de %d (%.1f%%)", com_reversa, m, 100.0 * com_reversa / m);
    if (dups > 0) printf("  [atencao: %d arestas duplicadas no arquivo]", dups);
    printf("\n");

    /* ---------- CFCs ---------- */
    int unitarias = 0, gigante = 0;
    for (int c = 0; c < s->n_comp; c++) {
        if (s->tam[c] == 1) unitarias++;
        if (s->tam[c] > s->tam[gigante]) gigante = c;
    }
    printf("\n== CFCs ==\n");
    printf("CFCs: %d | unitarias: %d | com mais de 1 vertice: %d\n",
           s->n_comp, unitarias, s->n_comp - unitarias);
    printf("maior CFC: %d vertices (%.1f%% da rede)\n", s->tam[gigante], 100.0 * s->tam[gigante] / n);

    /* ---------- armadilhas (CFCs terminais) ---------- */
    entra = calloc(s->n_comp, sizeof(char));
    rep = malloc(s->n_comp * sizeof(int));
    tam_multi = malloc(s->n_comp * sizeof(int));
    if (!entra || !rep || !tam_multi) { fprintf(stderr, "erro de alocacao\n"); free(lat); free(lon); goto fim; }
    for (int c = 0; c < s->n_comp; c++) rep[c] = -1;
    for (int v = 0; v < n; v++)
        if (rep[s->comp[v]] < 0) rep[s->comp[v]] = v;
    for (int i = 0; i < m; i++)
        if (s->comp[U[i]] != s->comp[V[i]]) entra[s->comp[V[i]]] = 1;

    int term_unit = 0, term_multi = 0, em_term = 0, term_com_entrada = 0;
    for (int c = 0; c < s->n_comp; c++) {
        if (!a->terminal[c]) continue;
        em_term += s->tam[c];
        if (entra[c]) term_com_entrada++;
        if (s->tam[c] == 1) term_unit++;
        else tam_multi[term_multi++] = s->tam[c];
    }
    qsort(tam_multi, term_multi, sizeof(int), cmp_int);

    /* alcançabilidade a partir (e em direção) da CFC gigante */
    fila = malloc(n * sizeof(int));
    alc_f = malloc(n);
    alc_r = malloc(n);
    if (!fila || !alc_f || !alc_r) { fprintf(stderr, "erro de alocacao\n"); free(lat); free(lon); goto fim; }
    int n_alc_f = bfs(l, rep[gigante], alc_f, fila);
    int n_alc_r = bfs(r, rep[gigante], alc_r, fila);
    int term_alcancaveis = 0;
    for (int c = 0; c < s->n_comp; c++)
        if (a->terminal[c] && alc_f[rep[c]]) term_alcancaveis++;

    printf("\n== Armadilhas (CFCs terminais) ==\n");
    printf("terminais: %d | unitarias: %d | maiores que 1: %d\n", a->n_terminais, term_unit, term_multi);
    printf("tamanhos das terminais maiores que 1: [");
    for (int i = 0; i < term_multi; i++) printf(i ? ", %d" : "%d", tam_multi[i]);
    printf("]\n");
    printf("vertices em terminais: %d (%.1f%% da rede)\n", em_term, 100.0 * em_term / n);
    printf("terminais com entrada vinda de outra CFC: %d | sem entrada externa: %d\n",
           term_com_entrada, a->n_terminais - term_com_entrada);
    printf("terminais alcancaveis a partir da CFC gigante: %d de %d\n", term_alcancaveis, a->n_terminais);
    printf("vertices alcancaveis a partir da gigante: %d | vertices que alcancam a gigante: %d\n",
           n_alc_f, n_alc_r);

    /* ---------- becos sem saída x borda do recorte ---------- */
    double lat0 = lat[0], lat1 = lat[0], lon0 = lon[0], lon1 = lon[0];
    for (int v = 1; v < n; v++) {
        if (lat[v] < lat0) lat0 = lat[v];
        if (lat[v] > lat1) lat1 = lat[v];
        if (lon[v] < lon0) lon0 = lon[v];
        if (lon[v] > lon1) lon1 = lon[v];
    }
    double m_lat = METROS_POR_GRAU;
    double m_lon = METROS_POR_GRAU * cos((lat0 + lat1) / 2.0 * acos(-1.0) / 180.0);

    dist = malloc(n * sizeof(double));
    dist_becos = malloc((sem_saida > 0 ? sem_saida : 1) * sizeof(double));
    if (!dist || !dist_becos) { fprintf(stderr, "erro de alocacao\n"); free(lat); free(lon); goto fim; }
    int n_becos = 0, ate50 = 0, ate100 = 0;
    for (int v = 0; v < n; v++) {
        double d = (lat[v] - lat0) * m_lat;
        double t;
        if ((t = (lat1 - lat[v]) * m_lat) < d) d = t;
        if ((t = (lon[v] - lon0) * m_lon) < d) d = t;
        if ((t = (lon1 - lon[v]) * m_lon) < d) d = t;
        dist[v] = d;
        if (lista_vizinhos(l, v) == NULL) {
            dist_becos[n_becos++] = d;
            if (d <= 50) ate50++;
            if (d <= 100) ate100++;
        }
    }
    printf("\n== Becos sem saida x borda do recorte ==\n");
    printf("recorte (pelos nos): %.2f km x %.2f km\n", (lat1 - lat0) * m_lat / 1000.0, (lon1 - lon0) * m_lon / 1000.0);
    double med_becos = n_becos ? mediana(dist_becos, n_becos) : 0.0;
    double med_todos = mediana(dist, n);
    printf("becos sem saida: %d | mediana da distancia a borda: %.0f m (todos os vertices: %.0f m)\n",
           n_becos, med_becos, med_todos);
    printf("a menos de 50 m da borda: %d | a menos de 100 m: %d\n", ate50, ate100);

    free(lat);
    free(lon);
    rc = 0;

fim:
    free(dist); free(dist_becos);
    free(fila); free(alc_f); free(alc_r);
    free(entra); free(rep); free(tam_multi); free(usos);
    armadilha_liberar(a);
    scc_liberar(s);
    lista_liberar(l);
    lista_liberar(r);
    free(U); free(V);
    free(osm.nos); free(osm.pares);
    return rc;
}

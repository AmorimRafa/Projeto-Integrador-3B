#include "metricas.h"

void cronometro_iniciar(Cronometro *c) {
    c->inicio = clock();
}

double cronometro_ms(Cronometro *c) {
    return ((double)(clock() - c->inicio)) * 1000.0 / CLOCKS_PER_SEC;
}

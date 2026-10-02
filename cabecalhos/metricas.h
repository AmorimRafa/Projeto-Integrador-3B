#ifndef METRICAS_H
#define METRICAS_H
#include <time.h>

/*
 * metricas.h — Medição de tempo de execução (Fase I: desempenho).
 */
typedef struct
{
    clock_t inicio;
} Cronometro;

void cronometro_iniciar(Cronometro *c);

/* Retorna o tempo decorrido em milissegundos desde o inicio. */
double cronometro_ms(Cronometro *c);

#endif
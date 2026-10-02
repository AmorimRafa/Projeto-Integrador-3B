# Testes de Desempenho — Definição

## Objetivo

Medir tempo e memória do pipeline (leitura → montagem → Tarjan → armadilhas)
variando o tamanho do grafo, para:

- validar a complexidade esperada O(V + E) do Tarjan;
- comparar lista vs matriz de adjacência (tempo de construção e memória);
- alimentar os gráficos da Fase I.

## Tamanhos definidos

| Teste | V (vértices) | E (arestas) | Observação |
|---|---|---|---|
| T1 | 100 | ~400 | toy grande |
| T2 | 500 | ~2.000 | |
| T3 | 1.000 | ~4.000 | |
| T4 | 5.000 | ~20.000 | |
| T5 | 10.000 | ~40.000 | |
| T6 | 50.000 | ~200.000 | opcional (se o tempo permitir) |

Densidade alvo: E ≈ 4·V (grafo esparso, típico de malha viária).

## Estrutura dos grafos gerados

Gerador `gerar_grafo.c` cria grafos direcionados com:

- 80% das arestas em ciclos (garantem CFCs grandes e várias armadilhas);
- 20% das arestas aleatórias `u -> v` (ligando componentes);
- formato edge list padrão (ver `documentacao/formato_entrada.md`).

## Métricas coletadas

Para cada (teste, representação):

- tempo de leitura+validação (ms);
- tempo do Tarjan (ms);
- tempo da análise de armadilhas (ms);
- memória da lista / da matriz (bytes);
- todas registradas em `resultados/resultados.csv`.

## Comando padrão

```bash
./grafo dados/gerados/t3.txt            # lista
./grafo dados/gerados/t3.txt --matriz   # matriz
./grafo dados/gerados/t3.txt --comparar # memória lado a lado
```

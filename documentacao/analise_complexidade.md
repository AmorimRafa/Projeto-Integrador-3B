# Análise de Complexidade

## Complexidade teórica

| Etapa | Lista de adjacência | Matriz de adjacência |
|---|---|---|
| Construção (inserir E arestas) | O(E) | O(V²) para alocar + O(E) |
| Memória | O(V + E) | O(V²) |
| Iterar vizinhos de u | O(grau(u)) | O(V) |
| Tarjan (CFC) | O(V + E) | O(V²) se implementado sobre a matriz |

O Tarjan visita cada vértice e cada aresta uma vez → **O(V + E)**, que é
linear no tamanho do grafo. A análise de armadilhas também é O(V + E)
(uma varredura de arestas para montar o DAG de condensação).

## Evidência experimental (T1–T5, do resultados/resultados.csv)

| Teste | V | E | Tarjan lista (ms) | Tarjan matriz (ms) | Análise armadilhas (ms) |
|---|---|---|---|---|---|
| T1 | 100 | 400 | 0.014 | 0.011 | ~0.005 |
| T2 | 500 | 2.000 | 0.056 | 0.048 | ~0.028 |
| T3 | 1.000 | 4.000 | 0.158 | 0.158 | ~0.06 |
| T4 | 5.000 | 20.000 | 1.130 | 2.441 | ~1.0 |
| T5 | 10.000 | 40.000 | 2.431 | 10.361 | ~5.4 |

### Leitura

- O tempo do Tarjan cresce aproximadamente junto com V+E (T5 tem ~12x os
  vértices de T3 e ~15x o tempo) → consistente com O(V + E).
- A matriz de adjacência confirma o custo quadrático: no modo matriz o
  tempo do Tarjan inclui a derivação da lista a partir da matriz
  (varredura V²), e cresce ~66x de T1 para T5, mais rápido que o linear.
- A memória da lista cresce com V+E (arestas = nós encadeados); a da
  matriz cresce com V² (4·V² bytes em int de 4 bytes + ponteiros),
  confirmando a vantagem da lista em malhas viárias (esparsas).

## Conclusão

Para grafos esparsos como malhas viárias (grau médio ~4), a lista de
adjacência é a estrutura adequada: memória O(V+E) e Tarjan O(V+E). A
matriz só seria competitiva em grafos densos, o que não é o caso.

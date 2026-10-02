# Modelo do Grafo — Malha Viária

## Grafo direcionado

A malha viária da cidade é modelada como um grafo direcionado **G = (V, E)**:

- **Vértices (V):** interseções/cruzamentos da cidade. Cada vértice recebe um
  identificador inteiro `0 .. n-1`.
- **Arestas (E):** trechos de rua *direcionados* entre duas interseções.
  - **Mão única:** uma aresta `u → v`.
  - **Mão dupla:** duas arestas, `u → v` e `v → u`.
- **Pesos:** não existem nesta fase (Fase I — topologia). O tempo de trânsito
  como peso entra na Fase II.

## Representações

O grafo será representado de duas formas, para comparação de desempenho e
memória:

1. **Lista de adjacência** — `include/lista_adj.h`
2. **Matriz de adjacência** — `include/matriz_adj.h`

Ambas implementam as mesmas operações básicas (inserir/remover vértice e
aresta, consultar vizinhos, liberar memória), o que permite comparar resultados
do algoritmo de CFC sobre as duas representações.

## Estruturas auxiliares

- `Grafo` (`include/graph.h`): contêiner com contagem de vértices/arestas e a
  representação escolhida.
- Tarjan (`include/scc.h`): encontra as Componentes Fortemente Conexas.

## Perguntas de negócio atendidas

- **"Armadilhas de trânsito":** identificadas via CFCs — bairros (conjuntos de
  vértices) dos quais não é possível sair e voltar ao ponto de origem. A
  definição formal de armadilha será detalhada na issue
  "DEFINIR O QUE É UMA ARMADILHA DE TRÂNSITO".

# Metodologia — Fase I

## Visão geral

Pipeline em C para análise topológica de malha viária modelada como grafo
direcionado G = (V, E), com identificação de "armadilhas de trânsito" via
Componentes Fortemente Conexas (CFC).

## Etapas

1. **Modelagem** (`documentacao/modelo.md`): interseções = vértices; trechos de
   rua = arestas dirigidas; mão dupla = duas arestas; Fase I sem pesos.
2. **Entrada** (`documentacao/formato_entrada.md`): edge list `V E` + `u v` por
   linha.
3. **Leitura e validação** (`fontes/io.c`): parser tolerante a comentários
   (`#`) e linhas em branco; validação de faixa dos vértices; contagem de
   auto-laços e arestas duplicadas.
4. **Representações** (`fontes/lista_adj.c`, `fontes/matriz_adj.c`): lista e
   matriz de adjacência com as mesmas operações; container `Grafo`
   (`fontes/graph.c`) despacha pela `Representacao` escolhida.
5. **CFC** (`fontes/scc.c`): Tarjan — índice de descoberta, low-link, pilha;
   uma passada inicializando cada vértice não visitado. O(V+E).
6. **Armadilhas** (`fontes/armadilha.c`): para cada CFC, verifica se existe
   aresta saindo dela no DAG de condensação; CFCs terminais = armadilhas;
   também classifica por tamanho e auto-laços.
7. **Métricas** (`fontes/metricas.c`, `fontes/registro.c`): tempo com `clock()`
   nas três etapas; memória estimada por estrutura; log em CSV.
8. **Testes** (`testes/`, `dados/toy/`): grafo toy com CFCs conhecidas
   (PASS: 3 CFCs) e validação cruzada lista × matriz.
9. **Desempenho** (`documentacao/testes_desempenho.md`): T1–T5 (100–10.000
   vértices), gráficos em `resultados/graficos/`.

## Decisões de projeto

- **Tarjan em vez de Kosaraju**: uma única DFS, sem transpor o grafo,
  mesma complexidade; escolhido por simplicidade de memória auxiliar.
- **Lista como representação principal**: malha viária é esparsa
  (grau médio ~4) → O(V+E) de memória vs O(V²) da matriz.
- **Docker** (`compose.yml`): ambiente reproduzível com `gcc:15`.
- **Código em português** nos identificadores/comentários (exigência da
  disciplina).

## Limitações

- A análise de armadilha identifica CFCs terminais; uma definição mais
  fina (ex.: quais vértices específicos ficam "presos") pode ser derivada.
- No modo `--matriz`, o Tarjan roda sobre uma lista derivada (a matriz é
  usada como referência de construção/memória).
- Dataset real integrado: centro de SP (`dados/real/sp_centro.txt`, ver
  `documentacao/resultados.md`). Outras regiões podem ser exportadas do
  OSM com `scripts/osm_para_grafo.py`.

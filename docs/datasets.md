# Pesquisa de Datasets de Malha Viária

## Opções encontradas

### 1. OSMnx Street Networks (Harvard Dataverse) — recomendada
- Repositório com +110 mil redes de ruas de cidades dos EUA, em grafos
  **direcionados** (one-way vira aresta única; via de mão dupla vira duas
  arestas), com atributos de nome, velocidade, comprimento.
- Formatos: GraphML, node/edge lists, shapefiles.
- Link: https://dataverse.harvard.edu/osmnx-street-networks
- Prós: já é direcionado (casa com "armadilhas de trânsito"), validado,
  tamanhos variados (cidades pequenas → testes; grandes → desempenho).
- Contras: cidades dos EUA; GraphML exige parser (conversão para nosso
  formato com script Python).

### 2. OSMnx (biblioteca Python) — para cidade brasileira
- `osmnx.graph_from_place("Cidade, UF, Brasil", network_type="drive")`
  gera o mesmo grafo direcionado a partir do OpenStreetMap.
- Permite usar a cidade do grupo (cenário mais "negócio").
- Contras: exige Python + download da internet; conversão necessária.

### 3. SNAP Road Networks (Stanford)
- roadNet-CA / roadNet-PA / roadNet-TX.
- **Não servem diretamente:** arestas **não direcionadas** (mão dupla em todos
  os trechos). Como "grafo direcionado" é requisito, o conceito de armadilha
  fica degenerado (todo vértice sairia e voltaria).
- Utilidade: fallback para testes de desempenho de estrutura de dados.

### 4. Yuxiang Zeng OSM road networks (China)
- Direcionados, com distâncias e tempos de viagem já calculados
  (`*.road-d`, `*.road-t`). Útil para a Fase II.

## Decisão

**Primária:** OSMnx (opção 1 ou 2), gerando edge list **direcionado** no
formato definido em `docs/formato_entrada.md`.

**Pipeline de conversão** (a implementar em `scripts/`):

```
GraphML / osmnx graph → script Python → datasets/real/<cidade>.txt
formato: "V E" na primeira linha, depois "u v" por aresta
```

**Validação** (issue VALIDAR O DATASET ESCOLHIDO):
- contar vértices/arestas e conferir com o relatório de origem;
- garantir que os IDs estejam em `[0, V)`;
- verificar conectividade e a "largest SCC";
- rodar o CFC e conferir se o número de componentes é plausível.

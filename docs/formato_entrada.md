# Formato de Entrada

Arquivo de texto (`.txt`), uma aresta por linha, no formato **edge list**:

```
<n_vertices> <n_arestas>
<u1> <v1>
<u2> <v2>
...
```

## Exemplo

```
4 5
0 1
1 2
2 0
2 3
3 3
```

Grafo: `0 -> 1 -> 2 -> 0` (CFC de tamanho 3), `2 -> 3`, auto-laço `3 -> 3`.

## Regras

- Vértices são inteiros em `[0, n_vertices)`.
- Cada linha de aresta representa uma direção `u -> v`.
- Rua de mão dupla: duas linhas, `u v` e `v u`.
- Linhas em branco ou começando com `#` são ignoradas (comentários).
- Arestas duplicadas são permitidas na entrada; a implementação pode
  deduplicar ou contar multiplicidade (documentar na implementação).

## Arquivos

- `datasets/toy/` — grafos pequenos com CFCs conhecidos, para testes.
- `datasets/real/` — malha viária real selecionada na issue de dataset.

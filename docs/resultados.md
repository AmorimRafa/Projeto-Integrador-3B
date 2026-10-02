# Resultados — Fase I

## Ambiente

- Compilador: gcc 15 (docker `gcc:15`, `-Wall -Wextra`, sem warnings).
- Execução: `datasets/gerados/t1..t5.txt` (gerador próprio, E ≈ 4·V).

## Desempenho (results/resultados.csv)

| Teste | V | E | Tarjan lista (ms) | Tarjan matriz (ms) | Armadilhas (ms) |
|---|---|---|---|---|---|
| T1 | 100 | 400 | 0.014 | 0.011 | 0.005 |
| T2 | 500 | 2.000 | 0.056 | 0.048 | 0.028 |
| T3 | 1.000 | 4.000 | 0.158 | 0.158 | 0.063 |
| T4 | 5.000 | 20.000 | 1.130 | 2.441 | 1.04 |
| T5 | 10.000 | 40.000 | 2.431 | 10.361 | 5.42 |

- Tarjan escala ~linear com V+E (T5: 10x os vértices, ~15x o tempo do T3).
- Modo matriz degrada mais rápido (inclui derivação da lista, varredura V²):
  T5 matriz ≈ 4x o tempo da lista.

## Memória (toy cfc_simples.txt, V=5, E=6)

- Lista: ~152 bytes (struct + cabeças + 6 nós).
- Matriz: ~156 bytes (5×5 int = 100 B + ponteiros).
- Em T5, a matriz alocaria ~400 MB (10.000² × 4 B), inviável na prática;
  a lista ficaria ~1.3 MB. Por isso a lista é a escolha.

## CFCs e armadilhas nos grafos gerados

| Teste | CFCs | Armadilhas terminais |
|---|---|---|
| T1 | 1 | 1 |
| T2 | 39 | 23 |
| T3 | 18 | 7 |
| T4 | 114 | 66 |
| T5 | 233 | 129 |

Os grafos gerados têm muitas CFCs porque 80% das arestas formam ciclos
curtos — cenário artificial pensado para exercitar o algoritmo, não
representativo de uma cidade real. No dataset real (OSMnx), espera-se uma
CFC gigante (~toda a rede) e poucas armadilhas terminais.

## Gráfico

`results/graficos/tarjan.svg` — tempo do Tarjan × V, lista vs matriz.

## Conclusões da Fase I

1. O Tarjan detecta CFCs corretamente (teste toy PASS: 3 CFCs).
2. Armadilhas de trânsito = CFCs terminais no DAG de condensação; o
   detector as encontra em todos os testes.
3. Lista de adjacência é viável até grafos grandes (T5: ~2.4 ms, ~MB de
   memória); matriz é inviável em V grande.
4. Pendências: integrar dataset real (VALIDAR O DATASET ESCOLHIDO /
   EXECUTAR CFC NO DATASET REAL).

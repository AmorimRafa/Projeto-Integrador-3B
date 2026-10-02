# Resultados — Fase I

## Ambiente

- Compilador: gcc 15 (docker `gcc:15`, `-Wall -Wextra`, sem warnings).
- Execução: `dados/gerados/t1..t5.txt` (gerador próprio, E ≈ 4·V).

## Desempenho (resultados/resultados.csv)

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

`resultados/graficos/tarjan.svg` — tempo do Tarjan × V, lista vs matriz.

## Dataset real — centro de São Paulo (OSM)

Conversão de `dados/real/sp_centro.osm` (Overpass API, bbox do centro de
SP) para `dados/real/sp_centro.txt`:

| Métrica | Valor |
|---|---|
| Vértices (interseções) | 6.039 |
| Arestas (ruas dirigidas) | 8.154 |
| Auto-laços / duplicadas | 0 / 0 |
| Leitura + validação | ~86 ms |
| Memória da lista | 178.792 bytes |
| Tarjan | ~3.4 ms |
| CFCs | 979 |
| Maior CFC | 4.786 vértices (~79% da rede) |
| CFCs terminais (armadilhas) | 60 |

A CFC gigante confirma que o centro de SP é essencialmente uma única
região coesa para dirigir; as 60 CFCs terminais sinalizam pontos onde
quem entra não consegue sair/voltar pelo sentido das vias.

## Conclusões da Fase I

1. O Tarjan detecta CFCs corretamente (teste toy PASS: 3 CFCs).
2. Armadilhas de trânsito = CFCs terminais no DAG de condensação; o
   detector as encontra em todos os testes.
3. Lista de adjacência é viável até grafos grandes (T5: ~2.4 ms, ~MB de
   memória); matriz é inviável em V grande.
4. Dataset real integrado: `dados/real/sp_centro.txt` gerado do OSM por
   `scripts/osm_para_grafo.py`; CFC executado com sucesso.

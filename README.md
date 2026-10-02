
# 🚇 Projeto Integrador 3B — Transportes Urbanos

Projeto desenvolvido para a disciplina de **Teoria dos Grafos**, com o objetivo de aplicar conceitos de grafos na análise de problemas relacionados aos transportes urbanos e tráfego.

## 🎯 Objetivo

Modelar uma rede de transportes urbanos utilizando grafos e desenvolver soluções computacionais em linguagem C para analisar sua conectividade e propriedades estruturais.

## 📦 Escopo do Projeto

O projeto será desenvolvido em duas fases:

- **Fase I — Topologia e Conectividade:** construção das estruturas de dados, representação do grafo e análise de conectividade.
- **Fase II — Análise de Rotas e Otimização:** inclusão de pesos e custos para análise de rotas e problemas de otimização.

O problema específico está definido: **malha viária como grafo direcionado, com detecção de "armadilhas de trânsito" via CFCs (Tarjan)**.

## 🧩 Tecnologias

- Linguagem C (C11)
- Docker / docker compose (imagem `gcc:15`)
- Git e GitHub
- GitHub Issues e branches

## 🚀 Como compilar e executar

O build usa o docker compose que está em `../compose.yml` (na raiz de
`trafego-urbano/`):

```bash
cd ..
docker compose run --rm app bash
# dentro do container:
cd Projeto-Integrador-3B
gcc -Wall -Wextra -std=c11 -Iinclude -o grafo \
  src/main.c src/io.c src/graph.c src/lista_adj.c src/matriz_adj.c \
  src/scc.c src/armadilha.c src/dfs.c src/metricas.c src/registro.c
./grafo datasets/toy/cfc_simples.txt            # lista
./grafo datasets/toy/cfc_simples.txt --matriz   # matriz
./grafo datasets/toy/cfc_simples.txt --comparar # memória lado a lado
```

Testes:

```bash
gcc -Wall -Wextra -Iinclude -o /tmp/t1 tests/test_cfc.c src/io.c src/lista_adj.c src/matriz_adj.c src/graph.c src/scc.c src/dfs.c
/tmp/t1 datasets/toy/cfc_simples.txt 3
```

Gerar gráfico:

```bash
gcc -o /tmp/graf scripts/gerar_graficos.c
/tmp/graf results/resultados.csv results/graficos/tarjan.svg
```

## 📁 Estrutura

```
include/    headers (.h) — graph, lista_adj, matriz_adj, io, dfs, scc,
            armadilha, metricas, registro
src/        implementações (.c) correspondentes + main.c
tests/      programas de teste (test_cfc, test_armadilha)
datasets/   toy/ (toy graphs), gerados/ (grafos de desempenho), real/ (futuro)
docs/       modelo, formato, datasets, armadilha, testes, complexidade,
            metodologia, resultados
results/    resultados.csv + graficos/
scripts/    gerar_grafo.c, gerar_graficos.c
```

## 🔬 Fase I — Atividades Previstas

- Definição do problema e do dataset.
- Modelagem da rede de transportes como um grafo.
- Implementação de lista e matriz de adjacência.
- Implementação de algoritmos de busca.
- Análise de conectividade forte utilizando o algoritmo de Tarjan.
- Comparação de desempenho e consumo de memória.

## 👥 Integrantes

| Integrante | GitHub |
|---|---|
| Pedro Rocha | [Pedr-ol](https://github.com/Pedr-ol) |
| Rafael Amorim | [AmorimRafa](https://github.com/AmorimRafa) |
| Caio Leandro | [CFLeandro](https://github.com/CFLeandro) |
| Mauricio da Paixão | [maumaukkj](https://github.com/maumaukkj) |

## 📌 Status do Projeto

🚧 Fase I quase concluída — pendências: dataset real (VALIDAR + EXECUTAR CFC NO DATASET REAL) e entrega final.

## 📄 Documentação

O projeto contará com um artigo científico no padrão SBC, conforme as exigências da disciplina.


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

## 🚀 Como compilar e executar (do zero)

Pré-requisito: **Docker** instalado e rodando.

```bash
git clone https://github.com/AmorimRafa/Projeto-Integrador-3B.git
cd Projeto-Integrador-3B

# sobe um container gcc:15 com o repositório montado em /app
docker compose run --rm app bash
```

Dentro do container:

```bash
# compila o programa principal
gcc -Wall -Wextra -std=c11 -Icabecalhos -o grafo \
  fontes/main.c fontes/io.c fontes/graph.c fontes/lista_adj.c fontes/matriz_adj.c \
  fontes/scc.c fontes/armadilha.c fontes/dfs.c fontes/metricas.c fontes/registro.c

# executa (grafo toy)
./grafo dados/toy/cfc_simples.txt            # lista de adjacência
./grafo dados/toy/cfc_simples.txt --matriz   # matriz de adjacência
./grafo dados/toy/cfc_simples.txt --comparar # memória lado a lado

# executa no dataset real (centro de SP)
./grafo dados/real/sp_centro.txt
```

Testes:

```bash
gcc -Wall -Wextra -Icabecalhos -o /tmp/t1 testes/test_cfc.c fontes/io.c fontes/lista_adj.c fontes/matriz_adj.c fontes/graph.c fontes/scc.c fontes/dfs.c
/tmp/t1 dados/toy/cfc_simples.txt 3
gcc -Wall -Wextra -Icabecalhos -o /tmp/t2 testes/test_armadilha.c fontes/io.c fontes/lista_adj.c fontes/scc.c fontes/armadilha.c
/tmp/t2 dados/toy/cfc_simples.txt
```

Gerar gráfico:

```bash
gcc -o /tmp/graf scripts/gerar_graficos.c
/tmp/graf resultados/resultados.csv resultados/graficos/tarjan.svg
```

## 📁 Estrutura

```
cabecalhos/   headers (.h): graph, lista_adj, matriz_adj, io, dfs, scc,
              armadilha, metricas, registro
fontes/       implementações (.c) + main.c
testes/       programas de teste (test_cfc, test_armadilha)
dados/        toy/, gerados/ (desempenho), real/ (dataset OSM de SP)
documentacao/ modelo, formato, datasets, armadilha, testes, complexidade,
              metodologia, resultados
resultados/   resultados.csv + graficos/
scripts/      gerar_grafo.c, gerar_graficos.c, osm_para_grafo.py
compose.yml   ambiente docker (imagem gcc:15)
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

# "Armadilha de Trânsito" — Definição

Uma **armadilha de trânsito** é um conjunto maximal de bairros (vértices) do
qual o motorista não consegue sair sem passar por um trecho "proibido" no
sentido oposto — ou seja, entra mas não volta ao ponto de origem.

## Formalização via CFC

Seja G = (V, E) o grafo direcionado da malha. Uma componente fortemente
conexa (CFC) C ⊆ V é um conjunto maximal tal que, para todo par u, v ∈ C,
existe caminho u ⇝ v e v ⇝ u.

- **CFC trivial fortemente presa (armadilha pura):** CFC C tal que todo
  vértice de C não possui arestas de saída para fora de C no grafo de
  componentes (DAG de condensação). Um carro que entre em C nunca mais
  sai nem retorna a vértices fora de C.
- **Arestas de saída (escapes):** se a CFC C possui arestas saindo para
  outras CFCs sem caminho de retorno, ela é uma "armadilha parcial": a
  entrada nela pode ser sem volta.

## Classificação para análise

Para cada CFC C do grafo:

| Propriedade | Interpretação |
|---|---|
| Tamanho 1 e sem auto-laço | vértice isolado de trânsito (bairro sem retorno) |
| Tamanho 1 com auto-laço | bairro que só retorna a si mesmo |
| Tamanho > 1 | bairro coeso: dentro dele sempre há como voltar |
| Sem arestas de saída no DAG de condensação | armadilha terminal |
| Com arestas de saída | possíveis rotas de fuga para outros bairros |

## Pergunta de negócio respondida

> "Existem armadilhas de trânsito, ou seja, bairros onde um carro entra, mas
> por conta do sentido das vias, não consegue mais sair e voltar ao ponto de
> origem?"

Resposta: **sim, todo vértice cuja CFC não tem caminho de retorno ao seu
componente de origem**. Na prática: CFCs cujos componentes no DAG de
condensação não conseguem alcançar o componente de partida — caso típico de
bairros servidos por vias de mão única "aprisionadas" por arteriais de mão
única.

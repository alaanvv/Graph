# Implementação do algoritmo de Ford-Fulkerson

Aluno: **Alan Vale**  

---

## Como executar

1. Compile o código  
`gcc main.c`  
  
2. Rode o executável  
`./a.out`  

---

A entrada e feita por um arquivo `graph.ncol`, no formato  
NCOL, com cada linha descrevendo uma aresta no seguinte formato:  
`origem destino peso`  
  
O programa irá tratar o vértice 1 como origem e o de maior índice  
como destino.  
  
## Entrada e saída esperada

Usando o arquivo `graph.ncol` com conteúdo:

```
1 2 3
1 3 5
2 4 3
3 4 5
3 5 4
4 5 5
```

Espera-se a saída:

```
~/documents/Graph/Ford-Fulkerson$ gcc main.c && ./a.out
Fluxo máximo: 8
```

# Implementação do algoritmo de 

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
  
## Entrada e saída esperada

Usando o arquivo `graph.ncol` com conteúdo:

```
1 2 1
1 5 2
2 3 1
2 4 2
3 4 4
3 5 2
4 1 3
5 1 1
5 4 1
```

E colocando a origem como **1**, espera-se a saída:

```
~/documents/Graph$ ./a.out
Origin: 
> 1

0: ? [ DISCONNECTED ]
1: 0 [ 1 ]
2: 1 [ 2 <- 1 ]
3: 2 [ 3 <- 2 <- 1 ]
4: 3 [ 4 <- 2 <- 1 ]
5: 2 [ 5 <- 1 ]
```

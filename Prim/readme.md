# Implementação do algoritmo de Prim

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
1 2 4
1 6 5
2 3 7
2 6 3
3 4 5
3 7 6
4 5 3
5 7 2
5 8 4
6 7 7
6 9 5
7 8 6
8 9 8
```

Espera-se a saída:  

```
(0, 1, 4)
(1, 5, 3)
(5, 8, 5)
(1, 2, 7)
(2, 3, 5)
(3, 4, 3)
(4, 6, 2)
(4, 7, 4)
```

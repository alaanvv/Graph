# Implementação do grafo PERT

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
1 2 7
1 3 5
2 4 9
3 5 6
4 5 0
4 6 11
5 7 4
6 8 3
7 8 8
8 9 6
8 10 4
10 9 0
9 11 7
```

Espera-se a saída:  

```
Tempos mais cedo / mais tarde (* = Caminho critico):
1 = 0/0 *
2 = 7/7 *
3 = 5/12
4 = 16/16 *
5 = 16/18
6 = 27/27 *
7 = 20/22
8 = 30/30 *
9 = 36/36 *
10 = 34/36
11 = 43/43 *
```

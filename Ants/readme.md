# Implementação do algoritmo Ant Colony Optimization (ACO)

Aluno: **Alan Vale**  

---

## Como executar

1. Compile o código  
`gcc src/main.c -lm`  
  
2. Rode o executável  
`./a.out`  

---

A entrada e feita por um arquivo `graph.txt`, que descreva uma matriz 15x15, sendo a representação das conexões de um grafo.
  
O programa calcula a melhor rota que passe por todos vértices, saindo do índice *1* até o último índice, depois voltando ao vértice inicial.

---

## Gerando valores por experimento fatorial

Vou testar valores pra *alpha*, *beta*, e *rho*

- *alpha*: (0.1,  5.0)
- *beta*:  (1.0, 10.0)
- *rho*:   (0.1,  0.9)

| alpha | beta | rho | Resultado (Distância) |
| :---: | :--: | :-: | :-------------------: |
|  0.1  | 1.0  | 0.1 |          355          |
|  0.1  | 1.0  | 0.9 |          375          |
|  0.1  | 10.0 | 0.1 |          291          |
|  0.1  | 10.0 | 0.9 |          291          |
|  5.0  | 1.0  | 0.1 |          358          |
|  5.0  | 1.0  | 0.9 |          369          |
|  5.0  | 10.0 | 0.1 |          291          |
|  5.0  | 10.0 | 0.9 |          291          |

A combinação `alpha=0.1`, `beta=10.0` e `rho=0.1` foi escolhida

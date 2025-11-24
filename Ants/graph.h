#ifndef GRAPH_H
#define GRAPH_H

#include <stdlib.h>
#include <stdio.h>

#define ASSERT(x, ...) if (!(x)) { printf(__VA_ARGS__); printf("\n"); exit(1); }

// Representa um grafo usando matriz de adjacencia
typedef struct {
  int **data;
  int size;
} Graph;

// Cria um novo grafo a partir de um arquivo de entrada
static inline Graph *graph_new(const char *path, int n) {
  Graph *g = (Graph *)malloc(sizeof(Graph));
  g->size = n;

  g->data = (int **)malloc(sizeof(int *) * n);
  for (int i = 0; i < n; i++)
    g->data[i] = (int *)malloc(sizeof(int) * n);

  FILE *input = fopen(path, "r");
  ASSERT(input, "Arquivo nao encontrado");

  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      fscanf(input, "%d", &g->data[i][j]);

  fclose(input);
  return g;
}

static inline void graph_free(Graph *g) {
  for (int i = 0; i < g->size; i++)
    free(g->data[i]);
  free(g->data);
  free(g);
}

// Retorna o tamanho de uma aresta
static inline int graph_get(Graph *g, int a, int b) { return g->data[a][b]; }

// Verifica se existe uma aresta entre dois nos
static inline int graph_has(Graph *g, int a, int b) { return g->data[a][b] != 0; }

#endif

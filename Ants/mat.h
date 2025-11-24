#ifndef MAT_H
#define MAT_H

#include <stdlib.h>
#include <stdio.h>

#define ASSERT(x, ...) if (!(x)) { printf(__VA_ARGS__); printf("\n"); exit(1); }

typedef struct {
  double **m;
  int size;
} Matrix;

static inline Matrix *mat_new_from_file(char *path, int n) {
  Matrix *g = (Matrix *)malloc(sizeof(Matrix));
  g->size = n;

  g->m = (double **)malloc(sizeof(double *) * n);
  for (int i = 0; i < n; i++)
    g->m[i] = (double *)malloc(sizeof(double) * n);

  FILE *input = fopen(path, "r");
  ASSERT(input, "Arquivo nao encontrado");

  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      fscanf(input, "%lf", &g->m[i][j]);

  fclose(input);
  return g;
}

static inline Matrix *mat_new(int size) {
  Matrix *x = (Matrix *)malloc(sizeof(Matrix));
  x->size = size;
  x->m = (double **)malloc(sizeof(double *) * size);
  for (int i = 0; i < size; i++)
    x->m[i] = (double *)calloc(size, sizeof(double));
  return x;
}

static inline void mat_free(Matrix *x) {
  for (int i = 0; i < x->size; i++)
    free(x->m[i]);
  free(x->m);
  free(x);
}

// Soma a matriz b na matriz a
static inline void mat_add(Matrix *a, Matrix *b) {
  for (int i = 0; i < a->size; i++)
    for (int j = 0; j < a->size; j++)
      a->m[i][j] += b->m[i][j];
}

// Multiplica uma matriz por um escalar k
static inline void mat_scale(Matrix *a, double k) {
  for (int i = 0; i < a->size; i++)
    for (int j = 0; j < a->size; j++)
      a->m[i][j] *= k;
}

// Preenche uma matriz com um valor v
static inline void mat_fill(Matrix *a, double v) {
  for (int i = 0; i < a->size; i++)
    for (int j = 0; j < a->size; j++)
      a->m[i][j] = v;
}

#endif

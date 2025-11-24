#ifndef MAT_H
#define MAT_H

#include <stdlib.h>

typedef struct {
  double **m;
  int size;
} Matrix;

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

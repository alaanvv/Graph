#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>

typedef struct {
  int rows, cols;
  double **m;
} Matrix;

static inline Matrix* mat_new(int r, int c) {
  Matrix* x = malloc(sizeof(Matrix));
  x->rows = r;
  x->cols = c;
  x->m = malloc(sizeof(double *) * r);
  for (int i = 0; i < r; i++)
    x->m[i] = calloc(c, sizeof(double));
  return x;
}

static inline void mat_free(Matrix* x) {
  for (int i = 0; i < x->rows; i++)
    free(x->m[i]);
  free(x->m);
  free(x);
}

static inline void mat_set(Matrix* x, int r, int c, double v) {
  x->m[r][c] = v;
}

static inline double mat_get(Matrix* x, int r, int c) {
  return x->m[r][c];
}

static inline void mat_add(Matrix* a, const Matrix* b) {
  for (int i = 0; i < a->rows; i++)
    for (int j = 0; j < a->cols; j++)
      a->m[i][j] += b->m[i][j];
}

static inline void mat_scale(Matrix* a, double k) {
  for (int i = 0; i < a->rows; i++)
    for (int j = 0; j < a->cols; j++)
      a->m[i][j] *= k;
}

static inline void mat_fill(Matrix* a, double v) {
  for (int i = 0; i < a->rows; i++)
    for (int j = 0; j < a->cols; j++)
      a->m[i][j] = v;
}

#endif

#ifndef VEC_H
#define VEC_H

static inline void vec_fill(double *v, int size, double k) {
  for (int i = 0; i < size; i++)
    v[i] = k;
}

static inline double vec_sum(double *v, int size) {
  double sum = 0;
  for (int i = 0; i < size; i++)
    sum += v[i];
  return sum;
}

#endif

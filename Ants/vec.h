#ifndef VEC_H
#define VEC_H

#include <stdlib.h>
#include <stdio.h>

// Add an item to the vector
static inline void vec_fill(float* v, int size, float k) {
    for (int i = 0; i < size; i++)
      v[i] = k;
}

// Sum all elements in the vector
static inline float vec_sum(float* v, int size) {
    float sum = 0;
    for (int i = 0; i < size; i++)
        sum += v[i];
    return sum;
}

#endif

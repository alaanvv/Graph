#ifndef TOUR_H
#define TOUR_H

#include <stdlib.h>
#include "graph.h"

typedef struct {
  int* nodes;
  int count;
  int capacity;
  int length;
} Tour;

static inline Tour* tour_new(int capacity) {
  Tour* t = (Tour*) malloc(sizeof(Tour));
  t->nodes = (int*) malloc(sizeof(int) * capacity);
  t->count = 0;
  t->capacity = capacity;
  t->length = 0;
  return t;
}

static inline void tour_free(Tour* t) {
  free(t->nodes);
  free(t);
}

static inline void tour_reset(Tour* t) {
  t->count = 0;
  t->length = 0;
}

static inline void tour_add(Tour* t, int node, const Graph* g) {
  if (t->count > 0) {
    int prev = t->nodes[t->count - 1];
    t->length += graph_get((Graph*) g, prev, node);
  }
  t->nodes[t->count++] = node;
}

static inline int tour_close(Tour* t, const Graph* g) {
  if (t->count <= 1) return 0;
  int first = t->nodes[0];
  int last  = t->nodes[t->count - 1];
  t->length += graph_get((Graph*) g, last, first);
  return t->length;
}

static inline void tour_cpy(Tour* dst, const Tour* src) {
  dst->count = src->count;
  dst->length = src->length;
  for (int i = 0; i < src->count; i++)
    dst->nodes[i] = src->nodes[i];
}

static inline int tour_has(const Tour* t, int node) {
  for (int i = 0; i < t->count; i++)
    if (t->nodes[i] == node)
      return 1;
  return 0;
}

#endif

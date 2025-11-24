#ifndef TOUR_H
#define TOUR_H

#include "mat.h"

// Representa um caminho
typedef struct {
  int *nodes; // Lista de nos visitados
  int count;  // Numero de nos visitados
  int length; // Comprimento total do caminho
} Tour;

static inline Tour *tour_new(int size) {
  Tour *t = (Tour *)malloc(sizeof(Tour));
  t->nodes = (int *)malloc(sizeof(int) * size);
  t->length = 0;
  t->count = 0;
  return t;
}

static inline void tour_free(Tour *t) {
  free(t->nodes);
  free(t);
}

// Adiciona um no ao tour e atualiza o comprimento total baseado na aresta do grafo g
static inline void tour_add(Tour *t, int node, Matrix *g) {
  t->nodes[t->count++] = node;
  if (t->count > 1) t->length += g->m[t->nodes[t->count - 2]][node];
}

// Fecha o tour, adicionando o tamanho da aresta de volta ao no inicial
static inline int tour_end(Tour *t, Matrix *g) {
  if (t->count <= 1) return 0;
  t->length += g->m[t->nodes[t->count - 1]][t->nodes[0]];
  return t->length;
}

// Copia os dados de um tour para outro
static inline void tour_cpy(Tour *dst, Tour *src) {
  dst->count = src->count;
  dst->length = src->length;
  for (int i = 0; i < src->count; i++)
    dst->nodes[i] = src->nodes[i];
}

// Duplica os dados de um tour
static inline Tour *tour_dupe(Tour *src) {
  Tour* dupe = tour_new(src->count);
  tour_cpy(dupe, src);
  return dupe;
}

// Verifica se um no ja foi visitado
static inline int tour_has(Tour *t, int node) {
  for (int i = 0; i < t->count; i++)
    if (t->nodes[i] == node)
      return 1;
  return 0;
}

#endif

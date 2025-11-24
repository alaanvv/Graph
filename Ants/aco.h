#ifndef ACO_H
#define ACO_H

#include "graph.h"
#include "mat.h"
#include "tour.h"
#include "vec.h"
#include <limits.h>
#include <math.h>

typedef struct {
    int num_ants;
    int iterations;
    float alpha;
    float beta;
    float rho;
    float Q;
    float tau;

    Graph* g;
    Tour* best;
    Matrix* pher;
    Matrix* delta;
} ACO;

// Default constructor
static inline ACO* aco_new(Graph* g, int num_ants, int iterations, float alpha, float beta, float rho, float Q, float tau) {
    ACO* aco = (ACO*)malloc(sizeof(ACO));
    aco->num_ants = num_ants;
    aco->iterations = iterations;
    aco->alpha = alpha;
    aco->beta = beta;
    aco->rho = rho;
    aco->Q = Q;
    aco->tau = tau;
    aco->g = g;

    aco->pher = mat_new(g->size, g->size);
    mat_fill(aco->pher, tau);

    aco->delta = mat_new(g->size, g->size);
    mat_fill(aco->delta, 0);

    aco->best = tour_new(g->size);
    aco->best->length = INT_MAX;

    return aco;
}

static inline void aco_free(ACO* aco) {
    mat_free(aco->pher);
    mat_free(aco->delta);
    tour_free(aco->best);
    free(aco);
}

// Monta uma rota
static inline Tour *aco_make_tour(ACO* aco) {
  float *probs = (float *)malloc(aco->g->size * sizeof(float));
  Tour *t = tour_new(aco->g->size);
  int current = 0;
  tour_add(t, current, aco->g);

  // Loop ate passar por todos vertices
  while (t->count < aco->g->size) {
    vec_fill(probs, aco->g->size, 0);
    // Calcula a probabilidade de ir pra cada vertice
    for (int j = 0; j < aco->g->size; j++) {
      if (!tour_has(t, j) && graph_has(aco->g, current, j))
        probs[j] = pow(mat_get(aco->pher, current, j), aco->alpha) *
                   pow(1.0 / graph_get(aco->g, current, j), aco->beta);
    }

    float r = ((float)rand() / RAND_MAX) * vec_sum(probs, aco->g->size);
    if (!r)
      break;

    current = -1;
    float acc = 0;

    for (int k = 0; k < aco->g->size; k++) {
      acc += probs[k];
      if (acc >= r) {
        current = k;
        break;
      }
    }

    if (current == -1)
      break;
    tour_add(t, current, aco->g);
  }

  tour_close(t, aco->g);
  free(probs);
  return t;
}

// Deposita feromonios em todas arestas de uma rota
static inline void aco_deposit(ACO* aco, Tour *t) {
  for (int i = 0; i < t->count - 1; i++) {
    int a = t->nodes[i], b = t->nodes[i + 1];
    float v = mat_get(aco->delta, a, b) + aco->Q / t->length;
    mat_set(aco->delta, a, b, v);
    mat_set(aco->delta, b, a, v);
  }
}

// Atualiza os feromonios
static inline void aco_update_pher(ACO *aco) {
  // Evaporacao
  mat_scale(aco->pher, 1 - aco->rho);
  // Deposito de feromonio
  mat_add(aco->pher, aco->delta);
  // Garante que nenhum ficou com menos que o minimo
  for (int i = 0; i < aco->pher->rows; i++)
    for (int j = 0; j < aco->pher->cols; j++)
      aco->pher->m[i][j] = MAX(1e-16, aco->pher->m[i][j]);
}

// Algoritmo da colônia de formigas
// Recebe um grafo como parâmetro
static inline Tour *aco_run(Graph *g, int num_ants, int iterations, float alpha, float beta, float rho, float Q, float tau) {
  ACO* aco = aco_new(g, num_ants, iterations, alpha, beta, rho, Q, tau);

  // Roda as iteracoes do algoritmo
  for (int i = 0; i < aco->iterations; i++) {
    mat_scale(aco->delta, 0);

    // Pra cada iteracao, cada formiga cria um caminho
    for (int ant = 0; ant < num_ants; ant++) {
      Tour *cur = aco_make_tour(aco);
      if (cur->count == g->size) {
        if (cur->length < aco->best->length)
          tour_cpy(aco->best, cur);
        aco_deposit(aco, cur);
      }
      tour_free(cur);
    }

    aco_update_pher(aco);
  }

  Tour* result;
  tour_cpy(result, aco->best);
  aco_free(aco);
  return result;
}

#endif

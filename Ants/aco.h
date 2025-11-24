#ifndef ACO_H
#define ACO_H

#include <limits.h>
#include <math.h>
#include "vec.h"
#include "tour.h"

#define MAX(x, y) (x > y ? x : y)

// Estrutura com os dados do algoritmo
typedef struct {
  int iterations; // Numero de iteracoes
  int num_ants;   // Numero de formigas
  double alpha;   // Importancia do feromonio
  double beta;    // Importancia da distancia
  double rho;     // Taxa de evaporacao do feromonio
  double Q;       // Taxa de deposito de feromonio
  double tau;     // Valor inicial do feromonio

  Matrix *g;     // Grafo com as cidades
  Tour *best;    // Melhor caminho encontrado
  Matrix *pher;  // Matriz de feromonio
  Matrix *delta; // Matriz com o deposito de feromonio da iteracao
} ACO;

static inline ACO *aco_new(Matrix *g, int num_ants, int iterations, double alpha, double beta, double rho, double Q, double tau) {
  ACO *aco = (ACO *)malloc(sizeof(ACO));
  aco->iterations = iterations;
  aco->num_ants = num_ants;
  aco->alpha = alpha;
  aco->beta = beta;
  aco->rho = rho;
  aco->Q = Q;
  aco->tau = tau;
  aco->g = g;

  // Inicia a matriz de feromonio com o valor inicial tau
  aco->pher = mat_new(g->size);
  mat_fill(aco->pher, tau);

  // Inicia a matriz delta com zeros
  aco->delta = mat_new(g->size);
  mat_fill(aco->delta, 0);

  // Inicia o melhor caminho com comprimento infinito
  aco->best = tour_new(g->size);
  aco->best->length = INT_MAX;

  return aco;
}

static inline void aco_free(ACO *aco) {
  mat_free(aco->pher);
  mat_free(aco->delta);
  tour_free(aco->best);
  free(aco);
}

// ---

// Funcao que calcula as probabilidades de cada caminho
static inline void aco_calculate_probabilities(ACO *aco, Tour *t, int current, double *probs) {
  // Zera a matriz de probabilidades
  vec_fill(probs, aco->g->size, 0);

  // Para cada cidade vizinha nao visitada, calcula o numerador baseado no feromonio e na distancia
  for (int j = 0; j < aco->g->size; j++)
    if (!tour_has(t, j) && aco->g->m[current][j] != 0)
      probs[j] = pow(aco->pher->m[current][j], aco->alpha) * pow(1.0 / aco->g->m[current][j], aco->beta);

  // Divide as probabilidades pelo total (normaliza)
  double total_prob = vec_sum(probs, aco->g->size);
  if (total_prob > 0)
    for (int j = 0; j < aco->g->size; j++)
      probs[j] /= total_prob;
}

// Funcao que seleciona a proxima cidade com base nas probabilidades
static inline int aco_select_next_node(ACO *aco, double *probs) {
  // Gera um numero aleatorio de 0 a 1
  double r = (double)rand() / RAND_MAX;

  // Seleciona o item no vetor que contem r no seu range
  double acc = 0;
  for (int k = 0; k < aco->g->size; k++) {
    acc += probs[k];
    if (acc >= r) return k;
  }

  // Retorna -1 se nao teve caminho
  return -1;
}

// Constroi o caminho para uma unica formiga
static inline Tour *aco_make_tour(ACO *aco, int ant) {
  double *probs = (double *)malloc(aco->g->size * sizeof(double));

  // Inicia o tour numa cidade de forma que as formigas fiquem bem distribuidas
  Tour *t = tour_new(aco->g->size);
  int current = ant % aco->g->size;
  tour_add(t, current, aco->g);

  // Constroi o caminho visitando uma cidade por vez ate visitar todas
  while (t->count < aco->g->size) {
    aco_calculate_probabilities(aco, t, current, probs);
    current = aco_select_next_node(aco, probs);

    // A formiga ficou presa
    if (current == -1) break;

    tour_add(t, current, aco->g);
  }

  tour_end(t, aco->g);
  free(probs);
  return t;
}

// Deposita feromonio nas arestas do caminho de uma formiga
static inline void aco_deposit(ACO *aco, Tour *t) {
  for (int i = 0; i < t->count - 1; i++) {
    int a = t->nodes[i];
    int b = t->nodes[i + 1];

    // Feromonio inversamente proporcional ao comprimento do caminho
    double v = aco->delta->m[a][b] + aco->Q / t->length;
    aco->delta->m[a][b] = v;
    aco->delta->m[b][a] = v;
  }
}

// Atualiza a matriz de feromonio global
static inline void aco_update_pher(ACO *aco) {
  // Evaporacao
  mat_scale(aco->pher, 1 - aco->rho);
  // Deposito
  mat_add(aco->pher, aco->delta);

  // Garante que o feromonio nao caia abaixo do valor inicial
  for (int i = 0; i < aco->pher->size; i++)
    for (int j = 0; j < aco->pher->size; j++)
      aco->pher->m[i][j] = MAX(aco->tau, aco->pher->m[i][j]);
}

// Executa o algoritmo da colonia de formigas
static inline Tour *aco_run(Matrix *g, int num_ants, int iterations, double alpha, double beta, double rho, double Q, double tau) {
  ACO *aco = aco_new(g, num_ants, iterations, alpha, beta, rho, Q, tau);

  // Roda as iteracoes do algoritmo
  for (int i = 0; i < aco->iterations; i++) {
    mat_scale(aco->delta, 0); // Reseta a matriz de deposito

    // Cada formiga constroi um caminho
    for (int ant = 0; ant < num_ants; ant++) {
      Tour *cur = aco_make_tour(aco, ant);
      // Se a formiga encontrou um caminho
      if (cur->count == g->size) {
        // Se for o melhor, salva em best
        if (cur->length < aco->best->length) tour_cpy(aco->best, cur);
        // Deposita feromonio no caminho
        aco_deposit(aco, cur);
      }
      tour_free(cur);
    }

    // Atualiza o feromonio global
    aco_update_pher(aco);
  }

  Tour *result = tour_new(g->size);
  tour_cpy(result, aco->best);
  aco_free(aco);
  return result;
}

#endif

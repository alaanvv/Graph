#ifndef ACO_H
#define ACO_H

#include "graph.h"
#include "mat.h"
#include "tour.h"
#include "vec.h"
#include <limits.h>
#include <math.h>

#define MAX(x, y) (x > y ? x : y)

// Estrutura principal que contem todos os dados e parametros do algoritmo ACO
typedef struct {
    // Parametros do algoritmo
    int num_ants;     // Numero de formigas
    int iterations;   // Numero de iteracoes
    double alpha;      // Fator de importancia do feromonio
    double beta;       // Fator de importancia da distancia (heuristica)
    double rho;        // Taxa de evaporacao do feromonio
    double Q;          // Fator de deposito de feromonio
    double tau;        // Valor inicial do feromonio

    // Dados do problema
    Graph* g;         // Grafo com as cidades e distancias
    Tour* best;       // Melhor caminho encontrado ate agora
    Matrix* pher;     // Matriz de feromonio
    Matrix* delta;    // Matriz para acumular o deposito de feromonio da iteracao
} ACO;

// Construtor: aloca e inicializa uma nova estrutura ACO
static inline ACO* aco_new(Graph* g, int num_ants, int iterations, double alpha, double beta, double rho, double Q, double tau) {
    ACO* aco = (ACO*)malloc(sizeof(ACO));
    aco->num_ants = num_ants;
    aco->iterations = iterations;
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

// Libera toda a memoria alocada para a estrutura ACO
static inline void aco_free(ACO* aco) {
    mat_free(aco->pher);
    mat_free(aco->delta);
    tour_free(aco->best);
    free(aco);
}

// Funcao auxiliar para calcular as probabilidades de transicao
static inline void aco_calculate_probabilities(ACO* aco, Tour* t, int current, double* probs) {
    vec_fill(probs, aco->g->size, 0);
    // Para cada cidade vizinha, calcula a probabilidade baseada no feromonio e na distancia
    for (int j = 0; j < aco->g->size; j++) {
        if (!tour_has(t, j) && graph_has(aco->g, current, j)) {
            probs[j] = pow(aco->pher->m[current][j], aco->alpha) *
                       pow(1.0 / graph_get(aco->g, current, j), aco->beta);
        }
    }
}

// Funcao auxiliar para selecionar a proxima cidade com base nas probabilidades (roleta)
static inline int aco_select_next_city(ACO* aco, double* probs) {
    double total_prob = vec_sum(probs, aco->g->size);
    if (total_prob == 0) {
        return -1; // Sem caminho disponivel
    }

    double r = ((double)rand() / RAND_MAX) * total_prob;

    double acc = 0;
    for (int k = 0; k < aco->g->size; k++) {
        acc += probs[k];
        if (acc >= r) {
            return k;
        }
    }

    // Fallback para imprecisoes de ponto flutuante
    for (int k = aco->g->size - 1; k >= 0; k--) {
        if (probs[k] > 0) return k;
    }

    return -1;
}

// Constroi o caminho para uma unica formiga
static inline Tour *aco_make_tour(ACO* aco) {
  double *probs = (double *)malloc(aco->g->size * sizeof(double));
  Tour *t = tour_new(aco->g->size);
  int current = rand() % aco->g->size; // Comeca em uma cidade aleatoria
  tour_add(t, current, aco->g);

  // Constroi o caminho visitando uma cidade por vez
  while (t->count < aco->g->size) {
    aco_calculate_probabilities(aco, t, current, probs);
    
    current = aco_select_next_city(aco, probs);

    if (current == -1) {
      // A formiga ficou presa
      break;
    }
    tour_add(t, current, aco->g);
  }

  tour_end(t, aco->g);
  free(probs);
  return t;
}

// Deposita feromonio nas arestas do caminho de uma formiga
static inline void aco_deposit(ACO* aco, Tour *t) {
  for (int i = 0; i < t->count - 1; i++) {
    int a = t->nodes[i], b = t->nodes[i + 1];
    // A quantidade de feromonio e inversamente proporcional ao comprimento do caminho
    double v = aco->delta->m[a][b] + aco->Q / t->length;
    aco->delta->m[a][b] = v;
    aco->delta->m[b][a] = v;
  }
}

// Atualiza a matriz de feromonio globalmente apos uma iteracao
static inline void aco_update_pher(ACO *aco) {
  // Evaporacao: diminui o feromonio em todas as arestas
  mat_scale(aco->pher, 1 - aco->rho);
  // Deposito: adiciona o novo feromonio acumulado em delta
  mat_add(aco->pher, aco->delta);
  // Garante que o feromonio nao caia abaixo de um valor minimo
  for (int i = 0; i < aco->pher->size; i++)
    for (int j = 0; j < aco->pher->size; j++)
      aco->pher->m[i][j] = MAX(1e-16, aco->pher->m[i][j]);
}

// Executa o algoritmo da colonia de formigas
static inline Tour *aco_run(Graph *g, int num_ants, int iterations, double alpha, double beta, double rho, double Q, double tau) {
  ACO* aco = aco_new(g, num_ants, iterations, alpha, beta, rho, Q, tau);

  // Roda as iteracoes do algoritmo
  for (int i = 0; i < aco->iterations; i++) {
    mat_scale(aco->delta, 0); // Reseta a matriz de deposito a cada iteracao

    // Para cada iteracao, cada formiga constroi um caminho
    for (int ant = 0; ant < num_ants; ant++) {
      Tour *cur = aco_make_tour(aco);
      // Se a formiga completou o caminho
      if (cur->count == g->size) {
        // Se o caminho for o melhor ja encontrado, salva
        if (cur->length < aco->best->length)
          tour_cpy(aco->best, cur);
        // Deposita feromonio no caminho encontrado
        aco_deposit(aco, cur);
      }
      tour_free(cur);
    }

    // Atualiza a matriz de feromonio com base nos depositos da iteracao
    aco_update_pher(aco);
  }

  // Prepara o resultado para ser retornado
  Tour* result = tour_new(g->size);
  tour_cpy(result, aco->best);
  aco_free(aco); // Libera a estrutura ACO
  return result;
}

#endif

#include <time.h>
#include "aco.h"
#include "tour.h"
#include "mat.h"

#define ITERATIONS 200 // Numero de iteracoes
#define NUM_ANTS 15    // Numero de formigas
#define ALPHA 1        // Importancia do feromonio
#define BETA 5         // Importancia da distancia
#define RHO 0.5        // Taxa de evaporacao do feromonio
#define Q 100.0        // Taxa de deposito de feromonio
#define TAU 1e-15      // Valor inicial do feromonio

int main() {
  srand(time(0));

  Matrix *graph = mat_new_from_file("graph.txt", 15);

  Tour *best = aco_run(graph, NUM_ANTS, ITERATIONS, ALPHA, BETA, RHO, Q, TAU);
  tour_rotate_to_zero(best);

  if (best->length == INT_MAX)
    printf("Nenhum caminho encontrado\n");
  else {
    printf("Melhor caminho:\n");
    for (int i = 0; i < best->count; i++)
      printf("%s%d", i ? " -> " : "", best->nodes[i] + 1);
    printf(" -> %d (Tamanho %d)\n", best->nodes[0] + 1, best->length);
  }

  mat_free(graph);
  tour_free(best);
  return 0;
}

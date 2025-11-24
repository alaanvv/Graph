#include <stdlib.h>
#include <time.h>
#include "graph.h"
#include "tour.h"
#include "aco.h"

#define ALPHA 1
#define BETA 5
#define RHO 0.5
#define Q 100.0
#define TAU 1e-15
#define ITERATIONS 200

int main() {
  srand(time(0));

  Graph* graph = graph_new("graph.txt", 15);
  Tour* best = aco_run(graph);

  if (best->length == INT_MAX) printf("Nenhum caminho encontrado\n");
  else {
    printf("Melhor caminho: %d\n", best->length);
    for (int i = 0; i < best->count; i++)
      printf("%s%d", i ? " -> " : "", best->nodes[i]);
    printf(" -> %d\n", best->nodes[0]);
  }

  printf("%d\n", graph->size);
  graph_free(graph);
  tour_free(best);
  return 0;
}

#include <limits.h>
#include "tour.h"
#include "mat.h"

#define MIN(x, y) (x < y ? x : y)

Tour *farthest_insertion(Matrix *graph) {
  Tour *tour = tour_new(15);
  tour_add(tour, 0, graph);

  while (tour->count < 15) {
    int    farthest = -1;
    double max_dist = -1;

    // Encontrando o mais distante
    for (int i = 0; i < 15; i++) {
      if (tour_has(tour, i)) continue;
      double min_dist_to_tour = INT_MAX;

      for (int j = 0; j < tour->count; j++)
        min_dist_to_tour = MIN(min_dist_to_tour, graph->m[i][tour->nodes[j]]);

      if (min_dist_to_tour < max_dist) continue;
      max_dist = min_dist_to_tour;
      farthest = i;
    }

    int    best_pos = -1;
    double min_cost = INT_MAX;

    // Encontrando o melhor lugar pra inserir
    for (int i = 0; i < tour->count; i++) {
      int curr = tour->nodes[i];
      int next = tour->nodes[(i + 1) % tour->count];
      double cost_increase = graph->m[curr][farthest] + graph->m[farthest][next] - graph->m[curr][next];

      if (cost_increase > min_cost) continue;
      min_cost = cost_increase;
      best_pos = i + 1;
    }

    tour_insert(tour, farthest, best_pos);
  }

  tour_end(tour, graph);
  return tour;
}

int main() {
  Matrix *graph = mat_new_from_file("graph.txt", 15);

  Tour *best = farthest_insertion(graph);
  tour_rotate_to_zero(best);

  if (best->length == INT_MAX)
    printf("Nenhum caminho encontrado\n");
  else {
    printf("Melhor caminho (Tamanho %d)\n", best->length);
    for (int i = 0; i < best->count; i++)
      printf("%s%d", i ? " -> " : "", best->nodes[i] + 1);
    printf(" -> %d\n", best->nodes[0] + 1);
  }

  mat_free(graph);
  tour_free(best);
  return 0;
}

#include <stdlib.h>
#include <igraph.h>
#include <stdio.h>

#define ASSERT(x, ...) if (!(x)) { printf(__VA_ARGS__); printf("\n"); exit(1); }
#define MAX(x, y) (x > y ? x : y)
#define MIN(x, y) (x < y ? x : y)

int main() {
  FILE* input = fopen("graph.ncol", "r");
  ASSERT(input, "Cant open graph file");

  igraph_vector_int_t edges;
  igraph_vector_int_init(&edges, 0);

  igraph_vector_t capacity;
  igraph_vector_init(&capacity, 0);

  int size = 0;

  int from, to, weight;
  while (fscanf(input, "%d %d %d", &from, &to, &weight) == 3) {
    igraph_vector_int_push_back(&edges, from);
    igraph_vector_int_push_back(&edges, to);
    igraph_vector_push_back(&capacity, weight);
    size = MAX(size, MAX(from, to));
  }
  size++;

  fclose(input);

  // ---

  igraph_t g;
  igraph_create(&g, &edges, size, IGRAPH_DIRECTED);

  igraph_real_t value;
  igraph_maxflow_value(&g, &value, 1, size - 1, &capacity, NULL);

  printf("Fluxo máximo: %.0f\n", value);
  return 0;
}

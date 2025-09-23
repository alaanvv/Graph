#include <stdlib.h>
#include <stdio.h>

#define ASSERT(x, ...) if (!(x)) { printf(__VA_ARGS__); printf("\n"); exit(1); }
#define MAX(x, y) (x > y ? x : y)

typedef unsigned int uint;

// ---

int main() {
  FILE* input;

  input = fopen("graph.ncol", "r");
  ASSERT(input, "Cant open graph file");

  int graph[100][100] = { 0 };
  int size = 0;

  int from, to, weight;
  while (fscanf(input, "%d %d %d", &from, &to, &weight) == 3) {
    graph[from][to] = weight;
    size = MAX(size, MAX(from, to));
  }
  size++;

  fclose(input);

  // ---

  uint* distances = (uint*) malloc(sizeof(uint) * size);
  uint* visited   = (uint*) malloc(sizeof(uint) * size);
  uint* routes    = (uint*) malloc(sizeof(uint) * size);
  uint  visited_count = 0;

  for (int i = 0; i < size; i++) {
    distances[i] = -1;
    visited[i] = 0;
    routes[i] = i;
  }

  int origin;
  printf("Origin: \n> ");
  scanf("%d", &origin);

  distances[origin] = 0;

  // ---
  
  while (visited_count < size) {
    uint pivot_i = 0;
    for (int i = 0; i < size; i++)
      if (!visited[i] && distances[i] < distances[pivot_i])
        pivot_i = i;

    for (int i = 0; i < size; i++) {
      if (!graph[pivot_i][i]) continue;
      int new_distance = distances[pivot_i] + graph[pivot_i][i];
      if (new_distance >= distances[i]) continue;
      distances[i] = new_distance;
      routes[i] = pivot_i;
    }

    visited[pivot_i] = 1;
    visited_count++;
  }

  // ---

  printf("\n");
  for (int i = 0; i < size; i++) {
    if (distances[i] == -1) { printf("%u: ? [ DISCONNECTED ]\n", i); continue; }

    printf("%u: %u [ %u ", i, distances[i], i);

    int prev = (i == origin) ? -1 : routes[i];
    while (prev != -1) {
      printf("<- %d ", prev);
      prev = (prev == origin) ? -1 : routes[prev];
    }

    printf("]\n");
  }

  return 0;
}

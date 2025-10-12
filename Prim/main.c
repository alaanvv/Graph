#include <stdlib.h>
#include <stdio.h>

#define ASSERT(x, ...) if (!(x)) { printf(__VA_ARGS__); printf("\n"); exit(1); }
#define MAX(x, y) (x > y ? x : y)
#define MIN(x, y) (x < y ? x : y)

typedef unsigned int uint;

// ---

int main() {
  FILE* input;

  input = fopen("graph.ncol", "r");
  ASSERT(input, "Cant open graph file");

  int graph[100][100] = { 0 };
  for (int i = 0; i < 100; i++)
    for (int j = 0; j < 100; j++)
      graph[i][j] = -1;
  int size = 0;

  int from, to, weight;
  while (fscanf(input, "%d %d %d", &from, &to, &weight) == 3) {
    graph[from - 1][to - 1] = weight;
    size = MAX(size, MAX(from, to));
  }

  fclose(input);

  // ---

  uint* included = (uint*) malloc(sizeof(uint) * size);
  for (int i = 0; i < size; i++) included[i] = i == 0;
  uint included_count = 1;

  // ---

  while (included_count < size) {
    int min_i = -1, min_j = -1;
    for (int i = 0; i < size; i++)
      for (int j = 0; j < size; j++) 
        if (included[i] && !included[j] && graph[i][j] != -1 && (min_i == -1 || graph[i][j] < graph[min_i][min_j])) { 
          min_i = i; 
          min_j = j; 
          continue; 
        }
    included[min_j] = 1;
    included_count++;
    printf("(%d, %d, %d)\n", min_i, min_j, graph[min_i][min_j]);
  }

  return 0;
}

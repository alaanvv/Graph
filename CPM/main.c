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

  uint* earliest = (uint*) malloc(sizeof(uint) * size);
  uint* latest   = (uint*) malloc(sizeof(uint) * size);

  for (int i = 0; i < size; i++) {
    earliest[i] = 0;
    latest[i] = -1;
  }

  // ---

  for (int i = 0; i < size; i++)
    for (int j = 0; j < size; j++) 
      if (graph[i][j] >= 0)
        earliest[j] = MAX(earliest[j], earliest[i] + graph[i][j]);

  latest[size - 1] = earliest[size - 1];

  int repeat = 1;
  while (repeat) {
    repeat = 0;
    for (int i = size - 1; i >= 0; i--)
      for (int j = size - 1; j >= 0; j--) {
        if (graph[i][j] == -1 || latest[j] == -1) continue;
        uint new = latest[j] - graph[i][j];
        if (new >= latest[i]) continue;
        latest[i] = new;
        repeat = 1;
      }
  }  

  // ---

  printf("Tempos mais cedo / mais tarde (* = Caminho critico):\n");
  for (int i = 0; i < size; i++)
    printf("%d = %d/%d %c\n", i + 1, earliest[i], latest[i], (earliest[i] - latest[i]) ? ' ' : '*');

  return 0;
}

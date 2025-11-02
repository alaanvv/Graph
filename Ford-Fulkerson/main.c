#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define ASSERT(x, ...) if (!(x)) { printf(__VA_ARGS__); printf("\n"); exit(1); }
#define MAX(x, y) (x > y ? x : y)
#define MIN(x, y) (x < y ? x : y)

typedef unsigned int uint;

// ---

// Essa função pega um grafo *r* (tamanho *n*), e retorna se existe um caminho entre
// *s* e *t*, salvando o caminho em *p*. Essa função foi feita pelo ChatGPT, eu só
// adaptei pra que ela guardasse em *f* o fluxo capaz de passar por esse caminho.

int bfs(int r[100][100], int s, int t, int p[], int n, uint *f) {
  int v[100];
  memset(v, 0, sizeof(v));
  v[s] = 1;
  int q[100], h = 0, e = 0;
  q[e++] = s;
  *f = -1;

  while (h < e) {
    int u = q[h++];
    for (int i = 0; i < n; i++) {
      if (!v[i] && r[u][i] > 0) {
        p[i] = u;
        v[i] = 1;
        if (u == s) *f = MIN(r[u][i], *f);
        q[e++] = i;
        if (i == t) return 1;
      }
    }
  }
  return 0;
}

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

  int path[100];
  uint acc = 0, f;

  while (bfs(graph, 1, size - 1, path, size, &f)) {
    acc += f;
    for (int v = size - 1; v != 1; v = path[v]) {
      graph[path[v]][v] -= f;
      graph[v][path[v]] += f;
    }
  }

  printf("Fluxo máximo: %d\n", acc);
  return 0;
}


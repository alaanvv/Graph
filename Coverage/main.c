#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ASSERT(x, ...) if (!(x)) { printf(__VA_ARGS__); printf("\n"); exit(1); }
#define MAX(x, y) (x > y ? x : y)

typedef unsigned int uint;

typedef struct {
  uint has_camera;
  char label[64];
} Node;

typedef struct {
  uint nodes[2];
  char name[64];
} Edge;

// ---

Node nodes[1000];
uint nodes_size = 0;

Edge edges[1000];
uint edges_size = 0;

uint total_cameras = 0;

// ---


void parse_gml() {
  FILE* input = fopen("sjdr.gml", "r");
  ASSERT(input, "Cant open graph file");

  int id, source, target;
  char label[64], name[64];
  char buffer[256];

  while (fgets(buffer, sizeof buffer, input)) {
    char *p = buffer;
    while (*p == ' ' || *p == '\t') p++;

    if (strncmp(p, "node", 4) == 0) {
      while (fgets(buffer, sizeof buffer, input)) {
        char *q = buffer;
        while (*q == ' ' || *q == '\t') q++;
        if (*q == ']') break;

        if (strncmp(q, "id ", 3) == 0)
          sscanf(q, "id %d", &id);
        else if (strncmp(q, "label", 5) == 0)
          sscanf(q, "label \"%63[^\"]\"", label);
      }

      nodes[id].has_camera = 0;
      strcpy(nodes[id].label, label);
      nodes_size++;
    }

    else if (strncmp(p, "edge", 4) == 0) {
      while (fgets(buffer, sizeof buffer, input)) {
        char *q = buffer;
        while (*q == ' ' || *q == '\t') q++;
        if (*q == ']') break;

        if (strncmp(q, "source", 6) == 0)
          sscanf(q, "source %d", &source);
        else if (strncmp(q, "target", 6) == 0)
          sscanf(q, "target %d", &target);
        else if (strncmp(q, "name", 4) == 0)
          sscanf(q, "name \"%63[^\"]\"", name);
      }

      edges[edges_size].nodes[0] = source;
      edges[edges_size].nodes[1] = target;
      strcpy(edges[edges_size++].name, name);
    }
  }

  fclose(input);
}

uint is_watched(Edge edge) {
  return nodes[edge.nodes[0]].has_camera || nodes[edge.nodes[1]].has_camera;
}

void place_camera(Edge edge) {
  nodes[edge.nodes[0]].has_camera = 1;
}

uint is_all_watched() {
  for (uint i = 0; i < edges_size; i++)
    if (!is_watched(edges[i])) return 0;
  return 1;
}

// ---

/*
  Esse código ficaria mais rápido se eu estruturasse de uma forma que a partir de 
  uma esquina você pudesse puxar todas ruas que ela cobre, pois da forma atual, 
  sempre que eu preciso dessa informação eu tenho que loopar todas ruas pra 
  verificar se são cobertas pela esquina que eu busco.
 
  Mas vou manter assim porque a parte relacionada a grafos funciona perfeitamente 
  e eu tenho visão do que deveria ser mudado pra melhorar o código, só não tenho tempo.
*/

int main() {
  // Lê o arquivo
  parse_gml();

  // Loopa todas ruas
  for (uint i = 0; i < edges_size; i++) {
    // Se a rua já estiver vigiada, passa pra próxima
    if (is_watched(edges[i])) continue;
    // Coloca uma câmera na primeira esquina
    place_camera(edges[i]);
    total_cameras++;
  }

  // Loopa todas esquinas
  for (uint i = 0; i < nodes_size; i++) {
    // Se a esquina não tiver uma câmera, passa pra próxima
    if (!nodes[i].has_camera) continue;
    // Tira a câmera
    nodes[i].has_camera = 0;
    // Verifica se todas ruas continuam assistidas
    if (is_all_watched()) total_cameras--;
    // Senão, coloca a câmera de volta
    else nodes[i].has_camera = 1;
  }

  // ---

  for (uint i = 0; i < nodes_size; i++) {
    if (!nodes[i].has_camera) continue;
    printf("[ Câmera %s ]\n", nodes[i].label);

    char printed[8][64];
    uint size_printed = 0;

    for (uint j = 0; j < edges_size; j++) {
      if (edges[j].nodes[0] != i && edges[j].nodes[1] != i) continue;

      int already = 0;
      for (uint k = 0; k < size_printed; k++) {
        if (strcmp(printed[k], edges[j].name) == 0) {
          already = 1;
          break;
        }
      }
      if (already) continue;

      strcpy(printed[size_printed++], edges[j].name);
      printf("%s\n", edges[j].name);
    }

    printf("\n");
  }  

  printf("---\n\nTotal de câmeras: %d\n", total_cameras);

  return 0;
}

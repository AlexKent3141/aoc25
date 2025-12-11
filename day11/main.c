#include "assert.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"

#define MAX_NEIGHBOURS 50
#define MAX_NODES 10000
#define MAX_LINE_LEN (10 + MAX_NEIGHBOURS * 10)

struct node {
  int label;
  int num_neighbours;
  int neighbour_indices[MAX_NEIGHBOURS];
  uint64_t num_paths;
  uint64_t num_paths_with_dac;
  uint64_t num_paths_with_fft;
  uint64_t num_paths_with_dac_and_fft;
} nodes[MAX_NODES], *start, *target;

int num_nodes = 0;

void add_neighbour(struct node* parent, int child_index) {
  parent->neighbour_indices[parent->num_neighbours++] = child_index;
}

int name_to_label(const char* name) {
  return name[0] * (50 * 50) + name[1] * 50 + name[2];
}

int find_node_with_label(int label) {
  int i = 0;
  for (i = 0; i < num_nodes; ++i) {
    if (nodes[i].label == label) return i;
  }

  return -1;
}

void count_paths(struct node* current, struct node* target, int dac_label, int fft_label) {
  int i;
  uint64_t num_paths = 0;
  uint64_t num_paths_with_dac = 0;
  uint64_t num_paths_with_fft = 0;
  uint64_t num_paths_with_dac_and_fft = 0;
  struct node* n;

  int is_dac = current->label == dac_label;
  int is_fft = current->label == fft_label;

  if (current == target) {
    current->num_paths = 1;
    return;
  }

  if (current->num_paths > 0) {
    /* Already explored */
    return;
  }

  for (i = 0; i < current->num_neighbours; ++i) {
    n = &nodes[current->neighbour_indices[i]];
    count_paths(n, target, dac_label, fft_label);

    num_paths += n->num_paths;
    if (is_dac) {
      num_paths_with_dac += n->num_paths;
      num_paths_with_dac_and_fft += n->num_paths_with_fft;
    }
    else {
      num_paths_with_dac += n->num_paths_with_dac;
    }

    if (is_fft) {
      num_paths_with_fft += n->num_paths;
      num_paths_with_dac_and_fft += n->num_paths_with_dac;
    }
    else {
      num_paths_with_fft += n->num_paths_with_fft;
    }

    if (!is_dac && !is_fft) {
      num_paths_with_dac_and_fft += n->num_paths_with_dac_and_fft;
    }
  }

  if (current->num_paths == 0) {
    current->num_paths = num_paths;
    current->num_paths_with_dac = num_paths_with_dac;
    current->num_paths_with_fft = num_paths_with_fft;
    current->num_paths_with_dac_and_fft = num_paths_with_dac_and_fft;
  }
}

int main() {

  FILE* f = fopen("input.txt", "r");
  char line[MAX_LINE_LEN], *tok;
  int i, n;
  int label;
  struct node* parent = NULL, *current;
  int p1;
  uint64_t p2;
  const int you_label = name_to_label("you");
  const int out_label = name_to_label("out");
  const int svr_label = name_to_label("svr");
  const int dac = name_to_label("dac");
  const int fft = name_to_label("fft");

  memset(nodes, 0, MAX_NEIGHBOURS * sizeof(struct node));

  while (fgets(line, MAX_LINE_LEN, f)) {
    i = 0;
    tok = strtok(line, ": ");
    while (tok) {
      if (i == 0) {
        label = name_to_label(tok);
        n = find_node_with_label(label);
        if (n != -1) parent = &nodes[n];
        else {
          parent = &nodes[num_nodes++];
        }

        parent->label = label;
      }
      else {
        label = name_to_label(tok);
        n = find_node_with_label(label);
        if (n == -1) {
          nodes[num_nodes].label = label;
          n = num_nodes;
          ++num_nodes;
        }

        add_neighbour(parent, n);
      }

      ++i;

      tok = strtok(NULL, ": ");
    }

    printf("Label: %d, num children %d\n", parent->label, parent->num_neighbours);
  }

  fclose(f);

  /* Add a node for out. */

  /* Find the start */
  n = find_node_with_label(you_label);
  assert(n != -1);
  start = &nodes[n];

  n = find_node_with_label(out_label);
  assert(n != -1);
  target = &nodes[n];

  printf("Num nodes: %d, start %d\n", num_nodes, start->label);

  count_paths(start, target, dac, fft);
  p1 = start->num_paths;
  printf("P1: %d\n", p1);

  n = find_node_with_label(svr_label);
  assert(n != -1);
  start = &nodes[n];

  /* Reset the counts */
  for (i = 0; i < num_nodes; ++i) {
    current = &nodes[i];
    current->num_paths = 0;
    current->num_paths_with_dac = 0;
    current->num_paths_with_fft = 0;
    current->num_paths_with_dac_and_fft = 0;
  }

  count_paths(start, target, dac, fft);
  p2 = start->num_paths_with_dac_and_fft;

  printf("P2: %ld\n", p2);

  return 0;
}

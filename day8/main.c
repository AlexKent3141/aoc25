#include "float.h"
#include "math.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"

#define NUM_POINTS 1000
#define NUM_EDGES 1000

struct point {
  int x, y, z, considered;
  int num_neighbours;
  int neighbour_indices[NUM_POINTS];
} pts[NUM_POINTS];

/* Will sort these and keep a stack of edges so we know which to consider next. */
struct edge {
  int i, j;
  float length;
} edges[NUM_POINTS * NUM_POINTS];

int edge_stack_index = 0;

void add_edge(int n1, int n2) {
  struct point* p1 = &pts[n1];
  struct point* p2 = &pts[n2];
  p1->neighbour_indices[p1->num_neighbours++] = n2;
  p2->neighbour_indices[p2->num_neighbours++] = n1;
}

int is_connected(int n1, int n2) {
  const struct point* p = &pts[n1];
  int i;
  for (i = 0; i < p->num_neighbours; ++i) {
    if (p->neighbour_indices[i] == n2) return 1;
  }
  return 0;
}

float distance(struct point p1, struct point p2) {
  return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2));
}

int count_component(int start) {
  int size = 1, r, n;
  struct point* pt = &pts[start];

  pt->considered = 1;

  /* Check this node's connections. */
  for (r = 0; r < pt->num_neighbours; ++r) {
    n = pt->neighbour_indices[r];
    if (!pts[n].considered) {
      size += count_component(n);
    }
  }

  return size;
}

struct connection_data {
  int e;
  struct edge latest_edge;
};

void connect_until(
  struct connection_data* data,
  int (*condition)(struct connection_data*)) {

  struct edge current_edge;
  for (data->e = 0; !condition(data); ++data->e) {
    current_edge = edges[edge_stack_index++];
    while (is_connected(current_edge.i, current_edge.j)) {
      current_edge = edges[edge_stack_index++];
    }

    data->latest_edge = current_edge;

    add_edge(current_edge.i, current_edge.j);
  }
}

int edge_count_reached_condition(struct connection_data* data) {
  return data->e >= 1000;
}

int is_connected_condition(struct connection_data* data) {
  int i, cpt_size;
  (void)data;

  /* Is the graph now all connected? */
  for (i = 0; i < NUM_POINTS; ++i) pts[i].considered = 0;

  cpt_size = count_component(0);
  if (cpt_size == NUM_POINTS) return 1;

  return 0;
}

int edge_cmp(const void* a, const void* b) {
  const struct edge* e1 = a;
  const struct edge* e2 = b;

  float len1 = e1->length;
  float len2 = e2->length;

  return len1 > len2;
}

int int_cmp(const void* a, const void* b) {
  return *(const int*)a < *(const int*)b;
}

int main() {

  int i = 0, j;
  float dist;
  uint64_t cpt_sizes[NUM_POINTS];
  int num_cpts = 0;
  struct connection_data data;

  FILE* f = fopen("input.txt", "r");
  while (fscanf(f, "%d,%d,%d", &pts[i].x, &pts[i].y, &pts[i].z) == 3) ++i;

  fclose(f);

  /* Initialise the distance matrix and edges. */
  for (i = 0; i < NUM_POINTS; ++i) {
    for (j = 0; j < NUM_POINTS; ++j) {
      dist = i != j ? distance(pts[i], pts[j]) : FLT_MAX;

      edges[i * NUM_POINTS + j].i = i;
      edges[i * NUM_POINTS + j].j = j;
      edges[i * NUM_POINTS + j].length = dist;
    }
  }

  qsort(edges, NUM_POINTS * NUM_POINTS, sizeof(struct edge), edge_cmp);

  /* Create the first 1000 connections. */
  connect_until(&data, edge_count_reached_condition);

  /* Identify the size of each connected component. */
  for (i = 0; i < NUM_POINTS; ++i) {
    if (pts[i].considered) continue;
    cpt_sizes[num_cpts++] = count_component(i);
  }

  qsort(cpt_sizes, num_cpts, sizeof(uint64_t), int_cmp);

  /* Keep connecting until there is only one connected component. */
  connect_until(&data, is_connected_condition);

  printf("P1: %ld, P2: %ld\n",
    cpt_sizes[0] * cpt_sizes[1] * cpt_sizes[2],
    (uint64_t)pts[data.latest_edge.i].x * (uint64_t)pts[data.latest_edge.j].x);

  return 0;
}

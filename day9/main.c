#include "assert.h"
#include "stdint.h"
#include "stdio.h"

#define ABS(a) ((a) < 0 ? -(a) : (a))
#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) < (b) ? (b) : (a))

#define MAX_POINTS 1000

struct point {
  int64_t x, y;
} pts[MAX_POINTS];

enum edge_type {
  HORIZONTAL,
  VERTICAL
};

struct edge {
  enum edge_type type;
  int fixed_axis;
  int end1, end2;
} edges[MAX_POINTS];

int num_edges = 0;

void add_edge(struct point p1, struct point p2) {
  struct edge e;

  e.type = p1.x == p2.x ? VERTICAL : HORIZONTAL;

  if (e.type == VERTICAL) {
    e.fixed_axis = p1.x;
    e.end1 = MIN(p1.y, p2.y);
    e.end2 = MAX(p1.y, p2.y);
  }
  else {
    e.fixed_axis = p1.y;
    e.end1 = MIN(p1.x, p2.x);
    e.end2 = MAX(p1.x, p2.x);
  }

  edges[num_edges++] = e;
}

/* This needs to include the border of the rect. */
uint64_t rect_area(struct point p1, struct point p2) {
  return (ABS(p1.x - p2.x) + 1) * (ABS(p1.y - p2.y) + 1);
}

/* Allow overlap on the end points */
int overlapping(int x1, int x2, int y1, int y2) {
  assert(x1 <= x2 && y1 <= y2);
  if (x1 >= y1 && x1 < y2) return 1; /* Fully enclosed */
  if (x2 <= y2 && x2 > y1) return 1; /* Fully enclosed */
  if (x1 > y2) return 0;             /* X interval is right of Y */
  if (x2 < y1) return 0;             /* X interval is left of Y */
  if (x1 < y2 && x2 > y1) return 1;  /* Overlap but not just at end points */

  return 0;
}

int is_area_internal(struct point p1, struct point p2) {
  /* Does the rectangle spanning these points only include internal points? */
  /* Check this by seeing whether any edges are included in the area. */
  int i;
  int min_x = MIN(p1.x, p2.x);
  int max_x = MAX(p1.x, p2.x);
  int min_y = MIN(p1.y, p2.y);
  int max_y = MAX(p1.y, p2.y);
  struct edge e;
  for (i = 0; i < num_edges; ++i) {
    e = edges[i];

    if (e.type == HORIZONTAL) {
      if (e.fixed_axis <= min_y || e.fixed_axis >= max_y) continue;
      if (overlapping(e.end1, e.end2, min_x, max_x)) return 0;
    }
    else {
      if (e.fixed_axis <= min_x || e.fixed_axis >= max_x) continue;
      if (overlapping(e.end1, e.end2, min_y, max_y)) return 0;
    }
  }

  return 1;
}

int main() {

  int i, j;
  int num_pts = 0;
  uint64_t p1 = 0, p2 = 0, area;
  FILE* f = fopen("input.txt", "r");

  while (fscanf(f, "%ld,%ld", &pts[num_pts].x, &pts[num_pts].y) == 2) {

    if (num_pts > 0) {
      /* Construct new edge with the previous point. */
      add_edge(pts[num_pts], pts[num_pts - 1]);
    }

    ++num_pts;
  }

  fclose(f);

  /* Final edge connecting back to the start. */
  add_edge(pts[num_pts - 1], pts[0]);

  for (i = 0; i < num_pts; ++i) {
    for (j = i + 1; j < num_pts; ++j) {
      area = rect_area(pts[i], pts[j]);
      p1 = MAX(p1, area);

      if (area > p2 && is_area_internal(pts[i], pts[j])) {
        p2 = area;
      }
    }
  }

  printf("P1: %ld P2: %ld\n", p1, p2);

  return 0;
}

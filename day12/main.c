#include "assert.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"

#define MAX_LINE_LEN 100
#define NUM_SHAPES 6

struct shape {
  char rows[3][5];
} shapes[NUM_SHAPES], *shape;

int main() {

  FILE* f = fopen("input.txt", "r");
  char line[MAX_LINE_LEN];
  int shape_index = 0;
  int is_shape;
  int area;
  int rows, cols;
  int counts[6];
  int totals;
  int p1 = 0;

  while (fgets(line, MAX_LINE_LEN, f)) {
    if (line[0] == '\n') continue;

    is_shape = strlen(line) < 5;

    if (is_shape) {
      shape = &shapes[shape_index++];

      /* Parse the next few rows into this shape. */
      assert(fgets(shape->rows[0], 5, f) != NULL);
      assert(fgets(shape->rows[1], 5, f) != NULL);
      assert(fgets(shape->rows[2], 5, f) != NULL);
    }
    else {
      /* Solve this packing problem. */
      assert(sscanf(line, "%dx%d: %d %d %d %d %d %d", &rows, &cols,
        &counts[0], &counts[1], &counts[2], &counts[3], &counts[4], &counts[5]) > 0);

      totals = counts[0] + counts[1] + counts[2] + counts[3] + counts[4] + counts[5];

      printf("%d %d %d\n", rows, cols, totals);

      /* Heuristic: are there enough 3x3 areas to trivially support the shapes? */
      area = (rows / 3) * (cols / 3);
      if (totals > area) {
        /* In this case we potentially need to do some actual work. */
        puts("Not enough 3x3");
        continue;
      }

      puts("Trivially possible");

      ++p1;
    }
  }

  fclose(f);

  printf("%d\n", p1);

  return 0;
}

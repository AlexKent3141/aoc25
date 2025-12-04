#include "stdio.h"
#include "stdlib.h"

#define GRID_SIZE 136

/* Each row is GRID_SIZE + LF + NUL */
char grid[GRID_SIZE][GRID_SIZE + 2];

int count_neighbours(int x, int y) {
  int count = 0;
  int dx, dy, nx, ny;
  for (dx = -1; dx <= 1; ++dx) {
    for (dy = -1; dy <= 1; ++dy) {
      if (dx == 0 && dy == 0) continue;

      nx = x + dx;
      ny = y + dy;

      if (nx < 0 || nx >= GRID_SIZE || ny < 0 || ny >= GRID_SIZE) continue;

      count += grid[ny][nx] == '@';
    }
  }

  return count;
}

int main() {

  FILE* f = fopen("input.txt", "r");

  int i, j, p1 = 0, p2 = 0, has_changes;
  for (i = 0; i < GRID_SIZE; i++) {
    if (fgets(grid[i], GRID_SIZE + 2, f) == NULL) return -1;
  }

  fclose(f);

  /* Part 1 */
  for (i = 0; i < GRID_SIZE; ++i) {
    for (j = 0; j < GRID_SIZE; ++j) {
      if (grid[j][i] == '@' && count_neighbours(i, j) < 4) {
        ++p1;
      }
    }
  }

  /* Part 2 */
  has_changes = 1;
  while (has_changes) {
    has_changes = 0;

    for (i = 0; i < GRID_SIZE; ++i) {
      for (j = 0; j < GRID_SIZE; ++j) {
        if (grid[j][i] == '@' && count_neighbours(i, j) < 4) {
          ++p2;
          grid[j][i] = '.';
          has_changes = 1;
        }
      }
    }
  }

  printf("P1: %d, P2: %d\n", p1, p2);

  return 0;
}

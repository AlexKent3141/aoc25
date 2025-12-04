#include "grid_helpers.h"

#include "stdio.h"
#include "stdlib.h"

#define GRID_SIZE 136

/* Each row is GRID_SIZE + LF + NUL */
char grid[GRID_SIZE][GRID_SIZE + 2];

int count_neighbours(int x, int y) {
  int count = 0;
  int dx, dy, nx, ny;
  LOOP_2D(dx, -1, 2, dy, -1, 2)
    if (dx == 0 && dy == 0) continue;

    nx = x + dx;
    ny = y + dy;

    if (nx < 0 || nx >= GRID_SIZE || ny < 0 || ny >= GRID_SIZE) continue;

    count += grid[ny][nx] == '@';
  LOOP_2D_END

  return count;
}

int main() {

  FILE* f = fopen("input.txt", "r");
  int i, j, p1 = 0, p2 = 0, has_changes;

  READ_GRID(f, i, grid, GRID_SIZE)

  fclose(f);

  /* Part 1 */
  LOOP_2D(i, 0, GRID_SIZE, j, 0, GRID_SIZE)
    if (grid[j][i] == '@' && count_neighbours(i, j) < 4) ++p1;
  LOOP_2D_END

  /* Part 2 */
  has_changes = 1;
  while (has_changes) {
    has_changes = 0;

    LOOP_2D(i, 0, GRID_SIZE, j, 0, GRID_SIZE)
      if (grid[j][i] == '@' && count_neighbours(i, j) < 4) {
        ++p2;
        grid[j][i] = '.';
        has_changes = 1;
      }
    LOOP_2D_END
  }

  printf("P1: %d, P2: %d\n", p1, p2);

  return 0;
}

#include "stdio.h"
#include "stdlib.h"

int main() {

  FILE* f = fopen("input.txt", "r");

  int end_zeroes = 0;
  int click_zeroes = 0;

  int loc = 50;
  char line[10];
  int prev;
  while (fgets(line, 10, f)) {
    char dir = line[0];
    int count = atoi(line + 1);

    click_zeroes += count / 100;
    count %= 100;

    prev = loc;

    loc += (dir == 'L' ? -count : count);

    /* Note: special case if we're already on zero */
    if (prev != 0 && (loc > 99 || loc < 1)) {
      ++click_zeroes;
    }

    if (loc < 0) loc += 100;
    loc %= 100;

    if (loc == 0) ++end_zeroes;
  }

  printf("P1: %d, P2: %d\n", end_zeroes, click_zeroes);

  fclose(f);

  return 0;
}

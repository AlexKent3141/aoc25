#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define MAX_JOLTAGE_WINDOW 100
int window[MAX_JOLTAGE_WINDOW];

uint64_t max_joltage(const char* buf, int n) {
  int val, i, j, tmp;
  uint64_t total = 0;

  memset(window, 0, MAX_JOLTAGE_WINDOW * sizeof(int));

  for (i = strlen(buf) - 1; i >= 0; --i) {
    val = buf[i] - '0';

    /* Do the initial update if there's empty elements. */
    j = 0;
    while (j < n && window[j] == 0) ++j;

    if (j > 0) {
      window[j - 1] = val;
      continue;
    }

    /* Do a "cascade" */
    j = 0;
    while (j < n && val >= window[j]) {
      tmp = window[j];
      window[j] = val;

      val = tmp;
      ++j;
    }
  }

  /* Construct the result */
  for (j = 0; j < n; j++) {
    total *= 10;
    total += window[j];
  }

  return total;
}

/* 100 digits + LF + NUL */
#define MAX_LINE_LEN 100 + 2

int main() {

  FILE* f = fopen("input.txt", "r");

  char line[MAX_LINE_LEN];
  uint64_t p1 = 0, p2 = 0;

  while (fgets(line, MAX_LINE_LEN, f)) {
    /* Remove the LF */
    line[strlen(line) - 1] = 0;

    p1 += max_joltage(line, 2);
    p2 += max_joltage(line, 12);
  }

  printf("P1: %ld, P2: %ld\n", p1, p2);

  fclose(f);

  return 0;
}

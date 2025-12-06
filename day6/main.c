#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define NUM_LINES 5
#define MAX_LINE_LEN 10000

char lines[NUM_LINES][MAX_LINE_LEN];

void print_problem(int start, int end) {
  int r;
  for (r = 0; r < NUM_LINES; ++r) {
    fwrite(lines[r] + start, 1, end - start, stdout);
    puts("");
  }
  puts("");
}

int parse_col_num(int offset) {
  int val = 0, row;
  char c;
  for (row = 0; row < NUM_LINES - 1; ++row) {
    c = lines[row][offset];
    if (val > 0 && c == ' ') return val;
    if (c == ' ') continue;
    val *= 10;
    val += c - '0';
  }

  return val;
}

int main() {

  int empty_cols[1001]; /* These separate the problems */
  int all_empty;
  char op;
  size_t row, col, row_index = 0, num_problems = 0;
  uint64_t p1 = 0, p2 = 0, current;
  int start, i, val;

  FILE* f = fopen("input.txt", "r");
  while (fgets(lines[row_index], MAX_LINE_LEN, f)) {
    ++row_index;
  }

  fclose(f);

  /* Identify the empty columns separating the problems */
  for (col = 0; col < strlen(lines[0]); ++col) {
    all_empty = 1;
    for (row = 0; row < NUM_LINES && all_empty; ++row) {
      all_empty &= lines[row][col] == ' ';
    }

    if (all_empty) {
      empty_cols[num_problems] = col;
      ++num_problems;
    }
  }

  empty_cols[num_problems++] = strlen(lines[0]) - 1;

  start = 0;
  for (col = 0; col < num_problems; ++col) {
    printf("%d %d\n", start, empty_cols[col]);
    print_problem(start, empty_cols[col]);

    /* Do the part 1 & 2 calculations */
    op = lines[4][start];

    current = op == '+' ? 0 : 1;
    for (i = 0; i < NUM_LINES - 1; ++i) {
      val = atoi(&lines[i][start]);
      if (op == '*') current *= val;
      else current += val;
    }

    p1 += current;

    current = op == '+' ? 0 : 1;
    for (i = start; i < empty_cols[col]; ++i) {
      val = parse_col_num(i);
      if (op == '*') current *= val;
      else current += val;
    }

    p2 += current;

    start = empty_cols[col] + 1;
  }

  printf("P1: %lu P2: %lu\n", p1, p2);

  return 0;
}

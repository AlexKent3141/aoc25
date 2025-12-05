#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"

#define MAX(x, y) (x > y ? x : y)
#define MIN(x, y) (x < y ? x : y)

#define MAX_RANGES 1000
#define MAX_LINE_LEN 100

struct range {
  uint64_t lower, upper;
  int considered;
};

int has_overlap(struct range r1, struct range r2) {
  return MAX(r1.lower, r2.lower) <= MIN(r1.upper, r2.upper);
}

struct range combine(struct range r1, struct range r2) {
  struct range r;
  r.lower = MIN(r1.lower, r2.lower);
  r.upper = MAX(r1.upper, r2.upper);
  r.considered = 0;
  return r;
}

int main() {

  FILE* f = fopen("input.txt", "r");
  struct range ranges[MAX_RANGES] = {0}, *r;
  int i, j, has_changes = 1;
  int num_ranges = 0, reading_ranges = 1;
  uint64_t p1 = 0, p2 = 0, val;
  char line[MAX_LINE_LEN];

  while (fgets(line, MAX_LINE_LEN, f)) {
    if (line[0] == '\n') {
      reading_ranges = 0;
      continue;
    }

    if (reading_ranges) {
      r = &ranges[num_ranges++];
      sscanf(line, "%lu-%lu", &r->lower, &r->upper);
    }
    else {
      val = atoll(line);
      for (i = 0; i < num_ranges; ++i) {
        r = &ranges[i];
        if (val >= r->lower && val <= r->upper) {
          ++p1;
          break;
        }
      }
    }
  }

  fclose(f);

  /* Keep merging the ranges in-place until we can't make more progress. */
  while (has_changes) {
    has_changes = 0;

    for (i = 0; i < num_ranges; ++i) {
      /* Can this guy be combined into a prior range? */
      for (j = 0; j < i && !ranges[i].considered; ++j) {
        if (ranges[j].considered) continue;

        if (has_overlap(ranges[i], ranges[j])) {
          ranges[j] = combine(ranges[i], ranges[j]);
          has_changes = 1;
          ranges[i].considered = 1;
        }
      }
    }
  }

  for (i = 0; i < num_ranges; ++i) {
    r = &ranges[i];
    if (!r->considered) p2 += (r->upper - r->lower + 1);
  }

  printf("%lu %lu\n", p1, p2);

  return 0;
}

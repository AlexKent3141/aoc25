#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"

int is_invalid_p1(const char* buf) {
  int len = strlen(buf);
  if (len & 1) return 0;

  return strncmp(buf, buf + len / 2, len / 2) == 0;
}

int is_invalid_p2(const char* buf) {
  int len, d, is_match = 0, offset;

  len = strlen(buf);

  for (d = 1; d <= len / 2 && !is_match; ++d) {
    if (len % d) continue;
    offset = d;
    is_match = 1;
    while (offset < len && is_match) {
      is_match &= strncmp(buf, buf + offset, d) == 0;
      offset += d;
    }
  }

  return is_match;
}

void update_invalid_totals(uint64_t start, uint64_t end, uint64_t* p1, uint64_t* p2) {
  /* Do this in the most naive way possible by converting to a string */
  char buf[30];
  for (; start <= end; start++) {
    sprintf(buf, "%ld", start);

    if (is_invalid_p1(buf)) *p1 += start;
    if (is_invalid_p2(buf)) *p2 += start;
  }
}

int main() {

  FILE* f = fopen("input.txt", "r");

  char line[10000], *tok;
  uint64_t start, end, sum_p1 = 0, sum_p2 = 0;

  if (fgets(line, 10000, f) == NULL) return 1;

  tok = strtok(line, "-,");
  while (tok != NULL) {
    start = atoll(tok);

    tok = strtok(NULL, "-,");

    end = atoll(tok);

    update_invalid_totals(start, end, &sum_p1, &sum_p2);

    tok = strtok(NULL, "-,");
  }

  printf("P1: %ld, P2: %ld\n", sum_p1, sum_p2);

  fclose(f);

  return 0;
}

#include "stdint.h"
#include "stdio.h"
#include "string.h"

#define LINE_LEN 141

int main() {

  char line[LINE_LEN + 2];
  int row_index = 0, col;

  /* Terminating these guys with NUL to help with print debugging. */
  char beams[LINE_LEN + 1] = {0};
  char next_beams[LINE_LEN + 1] = {0};

  uint64_t beam_count[LINE_LEN] = {0};
  uint64_t next_beam_count[LINE_LEN] = {0};
  uint64_t p1 = 0, p2 = 0;

  FILE* f = fopen("input.txt", "r");

  memset(beams, ' ', LINE_LEN);
  while (fgets(line, LINE_LEN + 2, f)) {
    if (row_index == 0) {
      beams[70] = '|';
      beam_count[70] = 1;
    }
    else if ((row_index & 1) == 0) {
      /* Row with splitters. */
      memset(next_beams, ' ', LINE_LEN);
      memset(next_beam_count, 0, LINE_LEN * sizeof(uint64_t));

      for(col = 0; col < LINE_LEN; ++col) {
        if (line[col] == '^' && beams[col] == '|') {
          /* Split */
          next_beams[col - 1] = '|';
          next_beams[col + 1] = '|';
          ++p1;

          next_beam_count[col - 1] += beam_count[col];
          next_beam_count[col + 1] += beam_count[col];
        }
        else if (beams[col] == '|') {
          /* Continue */
          next_beams[col] = '|';
          next_beam_count[col] += beam_count[col];
        }
      }

      memcpy(beams, next_beams, LINE_LEN);
      memcpy(beam_count, next_beam_count, LINE_LEN * sizeof(uint64_t));
    }

    ++row_index;
  }

  fclose(f);

  for (col = 0; col < LINE_LEN; ++col) {
    p2 += beam_count[col];
  }

  printf("P1: %ld P2: %ld\n", p1, p2);

  return 0;
}

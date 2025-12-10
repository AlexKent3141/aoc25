#include "assert.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"

#define MAX_LINE_LEN 1000
#define MAX_ACTIONS 100

struct machine {
  int target;
  int num_actions;
  int actions[MAX_ACTIONS];
} machines[200], *m;

int min_steps(struct machine m) {

  struct queue_entry {
    int current;
    int depth;
  };

  struct queue_entry* q = malloc(100000 * sizeof(struct queue_entry));

  int q_size = 0;

  struct queue_entry head;
  int head_index = 0;

  int i;
  int next;

  q[q_size].current = 0;
  q[q_size].depth = 0;
  ++q_size;

  for (;;) {
    head = q[head_index];

    /* Generate neighbours and append to q */
    for (i = 0; i < m.num_actions; ++i) {

      next = head.current ^ m.actions[i];
      if (next == m.target) return head.depth + 1;

      q[q_size].current = next;
      q[q_size].depth = head.depth + 1;
      ++q_size;
    }

    ++head_index;
  }

  return -1;
}

int main() {

  FILE* f = fopen("input.txt", "r");
  char line[MAX_LINE_LEN], *tok;
  int num_machines = 0;
  int i, j;
  int action;
  int n;
  int p1 = 0;

  while (fgets(line, MAX_LINE_LEN, f)) {
    m = &machines[num_machines++];
    tok = strtok(line, " ");
    while (tok) {
      if (tok[0] == '[') {
        m->target = 0;
        for (i = 1; i < (int)strlen(tok); ++i) {
          if (tok[i] == '#') {
            m->target |= (1 << (i - 1));
          }
        }
      }
      else if (tok[0] == '(') {
        action = 0;
        for (i = 1; i < (int)strlen(tok); ++i) {
          if (isdigit(tok[i])) {
            action |= (1 << (tok[i] - '0'));
          }
        }

        m->actions[m->num_actions++] = action;
      }
      else {
        /* Skip joltage */
      }

      tok = strtok(NULL, " ");
    }
  }

  fclose(f);

  for (i = 0; i < num_machines; i++) {
    m = &machines[i];
    n = min_steps(*m);
    printf("%d\n", m->target);
    for (j = 0; j < m->num_actions; ++j) {
      printf("%d ", m->actions[j]);
    }
    printf("Steps: %d\n", n);
    p1 += n;
  }

  printf("P1: %d\n", p1);

  return 0;
}

#ifndef __GRID_HELPERS_H__
#define __GRID_HELPERS_H__

/* Read a 2D grid from file. Assume each row is padded with LF + NUL */
#define READ_GRID(F, I, GRID_2D, SIZE)                     \
  for (I = 0; I < SIZE; ++I) {                             \
    if (fgets(GRID_2D[I], SIZE + 2, F) == NULL) return -1; \
  }

#define LOOP_2D(I, I_START, I_END, J, J_START, J_END)      \
  for (I = I_START; I < I_END; ++I) {                      \
    for (J = J_START; J < J_END; ++J) {                    \

#define LOOP_2D_END }}

#endif /* __GRID_HELPERS_H__ */

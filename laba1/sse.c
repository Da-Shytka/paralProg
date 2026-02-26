#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define iterations_num 100000000

void sse(float a[], float b[], float c[]) {
  asm volatile (
                "movups %[a], %%xmm0\n"
                "movups %[b], %%xmm1\n"
                "mulps %%xmm1, %%xmm0\n"
                "movups %%xmm0, %[c]\n"
                :
                : [a]"m"(*a), [b]"m"(*b), [c]"m"(*c)
                : "%xmm0", "%xmm1");
  // for (int i = 0; i < 4; i++) {
  //   printf("%f ", c[i]);
  // }
  // printf("\n");
}

void consistent(float a[], float b[], float c[]) {
  for (int i = 0; i < 4; i++){
    c[i] = a[i] * b[i];
  }
  // for (int i = 0; i < 4; i++) {
  //   printf("%f ", c[i]);
  // }
  // printf("\n");
}

int main(int argc, char** argv) {
  float a[4], b[4], c[4];
  for (int i = 0; i < 4; i++) {
    a[i] = ((float)rand() / RAND_MAX) * 100.0f;
    b[i] = ((float)rand() / RAND_MAX) * 100.0f;
  }

  clock_t start, end;
  double time_spent;

  start = clock();
  for (int i = 0; i < iterations_num; i++) {
    sse(a, b, c);
  }
  end = clock();
  time_spent = (double)(end - start) / CLOCKS_PER_SEC;
  printf("sse: %.2f секунд\n", time_spent);

  start = clock();
  for (int i = 0; i < iterations_num; i++) {
    consistent(a, b, c);
  }
  end = clock();
  time_spent = (double)(end - start) / CLOCKS_PER_SEC;
  printf("consistent: %.2f секунд\n", time_spent);

  return 0;
}
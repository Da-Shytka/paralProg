#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

void *heavy_task(void *i) {
  int thread_num = *((int*) i);
  for (int i = 0; i < 1e8; i++) {
    sqrt(i);
  }
  // printf("\tThread #%d finished\n", thread_num);
  free(i);
}

//параллельная версия
void pthreads(int n) {

  pthread_t threads[n];

  for (int i = 0; i < n; i++) {
    int *thread_num = (int*) malloc(sizeof(int));
    *thread_num = i;

    if (pthread_create(&threads[i], NULL, heavy_task, thread_num) != 0) {
        fprintf(stderr, "pthread_create failed\n");
        exit(EXIT_FAILURE);
    }
  }

  for (int i = 0; i < n; i++) {
    pthread_join(threads[i], NULL); //ждем завершение работы всех потоков по очереди
  }
}

//последовательная версия
void sequential(int n) {
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < 1e8; i++) {
          sqrt(i);
        }
        // printf("\tTask #%d finished\n", j);
    }
}



int main(int argc, char** argv) {
  int n = atoi(argv[1]);
  struct timespec start, end;
  double elapsed;

  clock_gettime(CLOCK_REALTIME, &start);
  sequential(n);
  clock_gettime(CLOCK_REALTIME, &end);
  elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/1e9;
  printf("Последовательное выполнение заняло %.2f секунд\n", elapsed);

  clock_gettime(CLOCK_REALTIME, &start);
  pthreads(n);
  clock_gettime(CLOCK_REALTIME, &end);
  elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/1e9;
  printf("Параллельное выполнение заняло %.2f секунд\n", elapsed);

  return 0;
}
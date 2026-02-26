#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

#ifdef _OPENMP
#include <omp.h>
#endif

// OpenMP
void openmp(int n) {
    printf("\n--- OpenMP ---\n");    
    #pragma omp parallel for num_threads(n)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 1e9; j++) {
            sqrt(j);
        }
        for (int j = 0; j < 1e9; j++) {
          sqrt(j);
        }
        printf("\tThread #%d finished (task %d)\n", omp_get_thread_num(), i);
    }
}

// 2 лаба
void *heavy_task_pthread(void *i) {
  int thread_num = *((int*) i);
  for (int i = 0; i < 1e9; i++) {
    sqrt(i);
  }
  for (int i = 0; i < 1e9; i++) {
    sqrt(i);
}
  printf("\tThread #%d finished\n", thread_num);
  free(i);
}

void pthreads(int n) {
    printf("\n--- Pthreads ---\n");
    pthread_t threads[n];
    
    for (int i = 0; i < n; i++) {
        int *thread_num = (int*)malloc(sizeof(int));
        *thread_num = i;
        
        if (pthread_create(&threads[i], NULL, heavy_task_pthread, thread_num) != 0) {
            fprintf(stderr, "pthread_create failed\n");
            exit(EXIT_FAILURE);
        }
    }
    
    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }
}

//последовательная версия
void sequential(int n) {
    printf("\n--- Последовательная ---\n");
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < 1e9; i++) {
          sqrt(i);
        }
        for (int i = 0; i < 1e9; i++) {
          sqrt(i);
        }
        printf("\tTask #%d finished\n", j);
    }
}

int main(int argc, char** argv) {
    int n = atoi(argv[1]);
    // clock_t start, end;
    struct timespec start, end;
    double seq_time, pthread_time, omp_time;

    clock_gettime(CLOCK_REALTIME, &start);
    sequential(n);
    clock_gettime(CLOCK_REALTIME, &end);
    seq_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/1e9;

    clock_gettime(CLOCK_REALTIME, &start);
    pthreads(n);
    clock_gettime(CLOCK_REALTIME, &end);
    pthread_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/1e9;

    clock_gettime(CLOCK_REALTIME, &start);
    openmp(n);
    clock_gettime(CLOCK_REALTIME, &end);
    omp_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/1e9;
    
    printf("\n=== ИТОГОВОЕ СРАВНЕНИЕ ===\n");
    printf("Последовательное: %.2f сек\n", seq_time);
    printf("Pthreads:         %.2f сек\n", pthread_time);
    printf("OpenMP:           %.2f сек\n", omp_time);

    printf("\n=== УСКОРЕНИЕ ===\n");
    printf("OpenMP относительно последовательной: %.2fx\n", seq_time / omp_time);
    printf("OpenMP относительно Pthreads: %.2fx\n", pthread_time / omp_time);


    return 0;
}
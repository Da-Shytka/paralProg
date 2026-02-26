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
        for (int j = 0; j < 1e8; j++) {
            sqrt(j);
        }
        #pragma omp critical
        printf("\tThread #%d finished (task %d)\n", omp_get_thread_num(), i);
    }
}

// 2 лаба
void *heavy_task_pthread(void *i) {
  int thread_num = *((int*) i);
  for (int i = 0; i < 1e8; i++) {
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
        for (int i = 0; i < 1e8; i++) {
          sqrt(i);
        }
        printf("\tTask #%d finished\n", j);
    }
}

int main(int argc, char** argv) {
    int n = atoi(argv[1]);
    clock_t start, end;
    double seq_time, pthread_time, omp_time;

    start = clock();
    sequential(n);
    end = clock();
    seq_time = (double)(end - start) / CLOCKS_PER_SEC;

    start = clock();
    pthreads(n);
    end = clock();
    pthread_time = (double)(end - start) / CLOCKS_PER_SEC;

    start = clock();
    openmp(n);
    end = clock();
    omp_time = (double)(end - start) / CLOCKS_PER_SEC;
    
    printf("\n=== ИТОГОВОЕ СРАВНЕНИЕ ===\n");
    printf("Последовательное: %.2f сек\n", seq_time);
    printf("Pthreads:         %.2f сек\n", pthread_time);
    printf("OpenMP:           %.2f сек\n", omp_time);
    
    // Сравнение Pthreads и OpenMP
    printf("\n--- СРАВНЕНИЕ PTHREADS И OPENMP ---\n");
    if (pthread_time < omp_time) {
        printf("OpenMP медленнее Pthreads на %.2f%%\n", 
               (omp_time - pthread_time) / omp_time * 100);
    } else if (omp_time < pthread_time) {
        double diff_percent = (pthread_time - omp_time) / omp_time * 100;
        printf("OpenMP быстрее Pthreads на %.2f%%\n", diff_percent);
    } else {
        printf("OpenMP и Pthreads показали одинаковое время\n");
    }

    // Сравнение OpenMP с последовательной
    printf("\n--- СРАВНЕНИЕ OPENMP С ПОСЛЕДОВАТЕЛЬНОЙ ---\n");
    if (seq_time < omp_time) {
        printf("OpenMP медленнее последовательной на %.2f%%\n", 
               (omp_time - seq_time) / omp_time * 100);
    } else if (omp_time < seq_time) {
        double diff_percent = (seq_time - omp_time) / omp_time * 100;
        printf("OpenMP быстрее последовательной на %.2f%%\n", diff_percent);
    } else {
        printf("OpenMP и последовательная показали одинаковое время\n");
    }

    return 0;
}
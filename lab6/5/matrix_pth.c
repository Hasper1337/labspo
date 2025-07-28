#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 1000
#define NUM_THREADS 8

typedef struct {
    int thread_id;
    double (*A)[N];
    double (*B)[N];
    double (*C)[N];
} ThreadData;

void* multiply_partial(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int id = data->thread_id;
    int rows_per_thread = N / NUM_THREADS;
    int start = id * rows_per_thread;
    int end = (id == NUM_THREADS - 1) ? N : start + rows_per_thread;

    for (int i = start; i < end; ++i)
        for (int j = 0; j < N; ++j) {
            data->C[i][j] = 0;
            for (int k = 0; k < N; ++k)
                data->C[i][j] += data->A[i][k] * data->B[k][j];
        }

    pthread_exit(NULL);
}

int main() {
    static double A[N][N], B[N][N], C[N][N];
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j) {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
        }

    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_data[i].thread_id = i;
        thread_data[i].A = A;
        thread_data[i].B = B;
        thread_data[i].C = C;
        pthread_create(&threads[i], NULL, multiply_partial, &thread_data[i]);
    }

    for (int i = 0; i < NUM_THREADS; ++i)
        pthread_join(threads[i], NULL);

    return 0;
}


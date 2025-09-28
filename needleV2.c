// Version 2
// Parallel version with threads

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>

const double D = 1.0;
const double L = 1.0;

typedef struct {
    long long n;
    unsigned int seed;
    long long crosses;
} ThreadData;

void* thread_func(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    const double factor = 1.0 / RAND_MAX;
    long long local_crosses = 0;

    for (long long i = 0; i < data->n; ++i) {
        double y_center = (rand_r(&data->seed) * factor) * (D / 2.0);
        double theta = (rand_r(&data->seed) * factor) * (M_PI / 2.0);
        if (y_center <= (L / 2.0) * sin(theta)) {
            ++local_crosses;
        }
    }

    data->crosses = local_crosses;
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        return 1;
    }

    long long n = atoll(argv[1]);
    int num_threads = atoi(argv[2]);
    if (n <= 0 || num_threads <= 0) return 1;
    if (!(L > 0.0 && D > 0.0 && L <= D)) return 1;

    pthread_t threads[num_threads];
    ThreadData data[num_threads];

    long long throws_per_thread = n / num_threads;
    long long remainder = n % num_threads;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < num_threads; i++) {
        data[i].n = throws_per_thread + (i < remainder ? 1 : 0);
        data[i].seed = (unsigned int)time(NULL) ^ (i * 7919);
        data[i].crosses = 0;
        pthread_create(&threads[i], NULL, thread_func, &data[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    double elapsed = (end.tv_sec - start.tv_sec)
                   + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("%.9f\n", elapsed);

    return 0;
}

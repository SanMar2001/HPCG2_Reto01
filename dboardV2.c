// Version 2
// Parallelization with threads
// and printing the time elapsed

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>

typedef struct {
    long long start;
    long long end;
    long long hits;
    unsigned int seed;
} ThreadData;

void* worker(void* arg) {
    ThreadData* data = (ThreadData*) arg;
    long long local_hits = 0;
    const double factor = 1.0 / RAND_MAX;

    for (long long k = data->start; k < data->end; k++) {
        double x = rand_r(&data->seed) * factor;
        double y = rand_r(&data->seed) * factor;
        if (x*x + y*y <= 1.0) {
            local_hits++;
        }
    }

    data->hits = local_hits;
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        return 1;
    }

    long long n = atoll(argv[1]);
    int num_threads = atoi(argv[2]);
    if (n <= 0 || num_threads <= 0) {
        return 1;
    }

    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];

    long long chunk = n / num_threads;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < num_threads; i++) {
        thread_data[i].start = i * chunk;
        thread_data[i].end = (i == num_threads - 1) ? n : (i + 1) * chunk;
        thread_data[i].hits = 0;
        thread_data[i].seed = (unsigned int) time(NULL) ^ (i * 7919); 
        pthread_create(&threads[i], NULL, worker, &thread_data[i]);
    }

    long long total_hits = 0;
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
        total_hits += thread_data[i].hits;
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    double pi_approx = 4.0 * (double) total_hits / (double) n;
    double elapsed = (end.tv_sec - start.tv_sec)
                   + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("%.9f\n", elapsed);

    return 0;
}

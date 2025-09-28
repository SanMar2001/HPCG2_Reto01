// Version 3
// Parallelization with processes using fork
// and printing the time elapsed

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        return 1;
    }

    long long n = atoll(argv[1]);
    int num_procs = atoi(argv[2]);
    if (n <= 0 || num_procs <= 0) {
        return 1;
    }

    int pipes[num_procs][2];
    long long chunk = n / num_procs;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < num_procs; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            return 1;
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            return 1;
        }

        if (pid == 0) {  
            close(pipes[i][0]);

            long long local_hits = 0;
            const double factor = 1.0 / RAND_MAX;
            unsigned int seed = (unsigned int) time(NULL) ^ (i * 7919);

            long long start_idx = i * chunk;
            long long end_idx = (i == num_procs - 1) ? n : (i + 1) * chunk;

            for (long long k = start_idx; k < end_idx; k++) {
                double x = rand_r(&seed) * factor;
                double y = rand_r(&seed) * factor;
                if (x*x + y*y <= 1.0) {
                    local_hits++;
                }
            }

            write(pipes[i][1], &local_hits, sizeof(local_hits));
            close(pipes[i][1]);
            exit(0);
        }
        else {
            close(pipes[i][1]);
        }
    }

    long long total_hits = 0;
    for (int i = 0; i < num_procs; i++) {
        long long local_hits;
        read(pipes[i][0], &local_hits, sizeof(local_hits));
        close(pipes[i][0]);
        total_hits += local_hits;
    }

    for (int i = 0; i < num_procs; i++) {
        wait(NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    double pi_approx = 4.0 * (double) total_hits / (double) n;
    double elapsed = (end.tv_sec - start.tv_sec)
                   + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("%.9f\n", elapsed);

    return 0;
}

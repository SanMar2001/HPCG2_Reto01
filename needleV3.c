// Version 3
// Parallel version with forked processes

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>

const double D = 1.0;
const double L = 1.0;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        return 1;
    }

    long long n = atoll(argv[1]);
    int num_procs = atoi(argv[2]);
    if (n <= 0 || num_procs <= 0) return 1;
    if (!(L > 0.0 && D > 0.0 && L <= D)) return 1;

    int pipes[num_procs][2];
    for (int i = 0; i < num_procs; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            return 1;
        }
    }

    long long throws_per_proc = n / num_procs;
    long long remainder = n % num_procs;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < num_procs; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            return 1;
        }
        if (pid == 0) {
            close(pipes[i][0]);

            long long local_n = throws_per_proc + (i < remainder ? 1 : 0);
            const double factor = 1.0 / RAND_MAX;
            long long local_crosses = 0;
            unsigned int seed = (unsigned int)time(NULL) ^ (i * 7919);

            for (long long j = 0; j < local_n; j++) {
                double y_center = (rand_r(&seed) * factor) * (D / 2.0);
                double theta = (rand_r(&seed) * factor) * (M_PI / 2.0);
                if (y_center <= (L / 2.0) * sin(theta)) {
                    ++local_crosses;
                }
            }

            if (write(pipes[i][1], &local_crosses, sizeof(local_crosses)) == -1) {
                perror("write");
            }
            close(pipes[i][1]);
            exit(0);
        } else {
            close(pipes[i][1]);
        }
    }

    long long total_crosses = 0;
    for (int i = 0; i < num_procs; i++) {
        long long local_crosses = 0;
        if (read(pipes[i][0], &local_crosses, sizeof(local_crosses)) == -1) {
            perror("read");
        }
        close(pipes[i][0]);
        total_crosses += local_crosses;
        wait(NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    double prob = (double)total_crosses / (double)n;
    double pi_estimate = (total_crosses == 0) ? INFINITY : (2.0 * L) / (D * prob);

    double elapsed = (end.tv_sec - start.tv_sec)
                   + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("%.9f\n", elapsed);

    return 0;
}

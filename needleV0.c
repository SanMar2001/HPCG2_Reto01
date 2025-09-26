// Version 0
// First version that use the secuential algorythm
// and print the estimate and the running time

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

const double D = 1.0;
const double L = 1.0;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        return 1;
    }

    long long n = atoll(argv[1]);
    if (n <= 0) return 1;
    if (!(L > 0.0 && D > 0.0 && L <= D)) return 1;

    const double factor = 1.0 / RAND_MAX;
    long long crosses = 0;
    srand((unsigned int)time(NULL));

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (long long i = 0; i < n; ++i) {
        double y_center = (rand() * factor) * (D / 2.0);
        double theta = (rand() * factor) * (M_PI / 2.0);
        if (y_center <= (L / 2.0) * sin(theta)) {
            ++crosses;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    double prob = (double)crosses / (double)n;
    double pi_estimate;
    if (crosses == 0) {
        pi_estimate = INFINITY;
    } else {
        pi_estimate = (2.0 * L) / (D * prob);
    }

    double elapsed = (end.tv_sec - start.tv_sec)
                   + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("%.9f\n", pi_estimate);
    printf("%.9f\n", elapsed);

    return 0;
}
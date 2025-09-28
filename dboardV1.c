// Version 0
// First version using only the secuencial method
// and printing the time elapsed

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        return 1;
    }

    long long n = atoll(argv[1]); 
    if (n <= 0) {
        return 1;
    }

    long long hits = 0;
    double x, y;
    const double factor = 1.0 / RAND_MAX;

    srand((unsigned int)time(NULL));

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (long long k = 0; k < n; k++) {
        x = rand() * factor;
        y = rand() * factor;
        if (x*x + y*y <= 1.0) {
            ++hits;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    
    double pi_approx = 4.0 * (double)hits / (double)n;
    double elapsed = (end.tv_sec - start.tv_sec) 
                   + (end.tv_nsec - start.tv_nsec) / 1e9;
    
    printf("%.9f\n", elapsed);

    return 0;
}
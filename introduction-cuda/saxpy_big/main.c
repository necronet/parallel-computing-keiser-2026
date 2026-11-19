#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N (1 << 26)  // 16,777,216 elements
#define REPEATS 500

double now_ms(void)
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec * 1000.0 + t.tv_nsec / 1000000.0;
}

void saxpy(int n, float a, const float *x, float *y)
{
    for (int i = 0; i < n; i++) {
        y[i] = a * x[i] + y[i];
    }
}

int main(void)
{
    size_t bytes = (size_t)N * sizeof(float);
    float *x = (float *)malloc(bytes);
    float *y = (float *)malloc(bytes);
    float a = 2.0f;

    for (int i = 0; i < N; i++) {
        x[i] = 1.0f;
        y[i] = 2.0f;
    }

    double start = now_ms();

    for (int r = 0; r < REPEATS; r++) {
        saxpy(N, a, x, y);
    }

    double elapsed_ms = now_ms() - start;

    // Consume every result outside the timed section.
    double checksum = 0.0;
    for (int i = 0; i < N; i++) {
        checksum += y[i];
    }

    printf("Elements: %d, repetitions: %d\n", N, REPEATS);
    printf("CPU total: %.3f ms\n", elapsed_ms);
    printf("CPU average per pass: %.3f ms\n", elapsed_ms / REPEATS);
    printf("First result: %.1f\n", y[0]);
    printf("Checksum: %.0f\n", checksum);

    free(x);
    free(y);
    return 0;
}

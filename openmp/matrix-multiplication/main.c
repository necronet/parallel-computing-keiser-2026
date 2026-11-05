#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

static int parse_positive(const char *text, int max)
{
    char *end;
    long value = strtol(text, &end, 10);
    return text[0] && *end == '\0' && value > 0 && value <= max ? (int)value : 0;
}

static void multiply_sequential(const double *a, const double *b, double *c, int n)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double sum = 0.0;
            for (int k = 0; k < n; k++) sum += a[i * n + k] * b[k * n + j];
            c[i * n + j] = sum;
        }
    }
}

static void multiply_parallel(const double *a, const double *b, double *c, int n)
{
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double sum = 0.0;
            for (int k = 0; k < n; k++) sum += a[i * n + k] * b[k * n + j];
            c[i * n + j] = sum;
        }
    }
}

static void print_matrix(const char *name, const double *matrix, int n)
{
    printf("%s:\n", name);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) printf("%8.0f", matrix[i * n + j]);
        putchar('\n');
    }
}

int main(int argc, char **argv)
{
    int n = argc > 1 ? parse_positive(argv[1], 3000) : 3;
    int threads = argc > 2 ? parse_positive(argv[2], 256) : 4;
    if (argc > 3 || n == 0 || threads == 0) {
        fprintf(stderr, "Usage: %s [size: 1-3000] [threads: 1-256]\n", argv[0]);
        return 1;
    }

    size_t count = (size_t)n * n;
    double *a = malloc(count * sizeof(*a));
    double *b = malloc(count * sizeof(*b));
    double *sequential = malloc(count * sizeof(*sequential));
    double *parallel = malloc(count * sizeof(*parallel));
    if (!a || !b || !sequential || !parallel) {
        fprintf(stderr, "Could not allocate matrices. Try a smaller size.\n");
        free(a); free(b); free(sequential); free(parallel);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a[i * n + j] = i + j + 1;
            b[i * n + j] = (i == j) ? 2.0 : 1.0;
        }
    }

    omp_set_dynamic(0);
    omp_set_num_threads(threads);

    double start = omp_get_wtime();
    multiply_sequential(a, b, sequential, n);
    double sequential_time = omp_get_wtime() - start;

    start = omp_get_wtime();
    multiply_parallel(a, b, parallel, n);
    double parallel_time = omp_get_wtime() - start;

    int correct = 1;
    for (size_t i = 0; i < count; i++) {
        /* Both versions use the same arithmetic order and integer-valued inputs. */
        if (sequential[i] != parallel[i]) {
            correct = 0;
            break;
        }
    }

    if (n <= 8) {
        print_matrix("A", a, n);
        print_matrix("B", b, n);
        print_matrix("C = A x B", parallel, n);
    }
    printf("Matrix size: %d x %d | Requested threads: %d\n", n, n, threads);
    printf("Results match: %s\n", correct ? "YES" : "NO");
    printf("Sequential: %.6f seconds\n", sequential_time);
    printf("OpenMP:     %.6f seconds\n", parallel_time);
    if (parallel_time > 0.0) printf("Speedup:    %.2fx\n", sequential_time / parallel_time);
    if (n <= 8) printf("Use a larger size, e.g. 800, to compare performance.\n");

    free(a); free(b); free(sequential); free(parallel);
    return correct ? 0 : 1;
}

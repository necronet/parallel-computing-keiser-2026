#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cuda_runtime.h>

#define N (1 << 24)  // 16,777,216 elements
#define REPEATS 20

double now_ms(void)
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec * 1000.0 + t.tv_nsec / 1000000.0;
}

__global__ void saxpy(int n, float a, const float *x, float *y)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < n) {
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

    float *d_x, *d_y;
    cudaMalloc((void **)&d_x, bytes);
    cudaMalloc((void **)&d_y, bytes);

    int threads = 256;
    int blocks = (N + threads - 1) / threads;

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    // Warm up the GPU before timing.
    cudaMemcpy(d_x, x, bytes, cudaMemcpyHostToDevice);
    cudaMemcpy(d_y, y, bytes, cudaMemcpyHostToDevice);
    saxpy<<<blocks, threads>>>(N, a, d_x, d_y);
    cudaDeviceSynchronize();

    double total_start = now_ms();

    // Reset inputs after warm-up. Include these transfers in total time.
    cudaMemcpy(d_x, x, bytes, cudaMemcpyHostToDevice);
    cudaMemcpy(d_y, y, bytes, cudaMemcpyHostToDevice);

    cudaEventRecord(start);

    for (int r = 0; r < REPEATS; r++) {
        saxpy<<<blocks, threads>>>(N, a, d_x, d_y);
    }

    cudaEventRecord(stop);
    cudaEventSynchronize(stop);

    cudaMemcpy(y, d_y, bytes, cudaMemcpyDeviceToHost);
    double total_ms = now_ms() - total_start;

    float kernel_ms;
    cudaEventElapsedTime(&kernel_ms, start, stop);

    // Consume every result outside the timed section.
    double checksum = 0.0;
    for (int i = 0; i < N; i++) {
        checksum += y[i];
    }

    printf("Elements: %d, repetitions: %d\n", N, REPEATS);
    printf("GPU kernels total: %.3f ms\n", kernel_ms);
    printf("GPU average per pass: %.3f ms\n", kernel_ms / REPEATS);
    printf("GPU including transfers: %.3f ms\n", total_ms);
    printf("First result: %.1f\n", y[0]);
    printf("Checksum: %.0f\n", checksum);

    cudaEventDestroy(start);
    cudaEventDestroy(stop);
    cudaFree(d_x);
    cudaFree(d_y);
    free(x);
    free(y);
    return 0;
}

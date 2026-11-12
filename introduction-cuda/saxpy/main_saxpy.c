// saxpy.cu
#include <stdio.h>
#include <cuda_runtime.h>

__global__ void saxpy(int n, float a, const float *x, float *y)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < n) {
        y[i] = a * x[i] + y[i];
    }
}

int main(void)
{
    int n = 4;
    float a = 2.0f;
    float x[] = {1, 2, 3, 4};
    float y[] = {10, 20, 30, 40};

    float *d_x, *d_y;
    size_t bytes = n * sizeof(float);

    // Allocate memory on the GPU.
    cudaMalloc((void **)&d_x, bytes);
    cudaMalloc((void **)&d_y, bytes);

    // Copy arrays to the GPU.
    cudaMemcpy(d_x, x, bytes, cudaMemcpyHostToDevice);
    cudaMemcpy(d_y, y, bytes, cudaMemcpyHostToDevice);

    // Run one block with four threads.
    saxpy<<<1, 4>>>(n, a, d_x, d_y);

    // Wait for the result and copy it back to the CPU.
    cudaMemcpy(y, d_y, bytes, cudaMemcpyDeviceToHost);

    for (int i = 0; i < n; i++) {
        printf("%.0f ", y[i]);
    }
    printf("\n");

    cudaFree(d_x);
    cudaFree(d_y);

    return 0;
}

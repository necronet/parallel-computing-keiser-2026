# Parallel image box blur

This example applies a **box blur** to the bundled `example.jpg`. For each pixel, it averages the pixels in a square neighborhood with radius 15 (up to 31 × 31 pixels). At image edges, it averages only valid neighbors. `#pragma omp parallel for schedule(static)` divides output rows among threads. Every thread reads the same source image, but writes to distinct pixels in the destination image, so the blur itself needs no lock or atomic operation.

The course syllabus includes simple parallel algorithms and performance evaluation. This example connects both: `main.c` runs the parallel blur; `benchmark.c` runs sequential and OpenMP versions and reports average times, speedup, and efficiency.

## Build and run

From this directory on Linux:

```sh
gcc -O2 -fopenmp main.c -o image-blur
gcc -O2 -fopenmp benchmark.c -o image-blur-benchmark
OMP_NUM_THREADS=4 ./image-blur
OMP_NUM_THREADS=4 ./image-blur-benchmark
```

For macOS, use the flags in the [OpenMP setup guide](../README.MD) with `main.c` or `benchmark.c`. Both programs require `example.jpg` in the **current working directory** and write `example_blurred.jpg` there. That output file is already tracked in this repository, so running either program will overwrite it. The bundled `stb_image.h` and `stb_image_write.h` handle image input and output; no separate image package is needed.

## Read the results

- `main.c` reports blur time in milliseconds and the maximum OpenMP thread count. It times the blur only, excluding image loading and JPEG writing.
- `benchmark.c` runs each method three times on the same image and reports their average blur times. **Speedup** is sequential time divided by parallel time. **Efficiency** is speedup divided by the reported maximum thread count, expressed as a percentage.
- The bundled image is 3888 × 4860 pixels, and this direct blur samples a large neighborhood for every pixel. It can take substantial time, especially in the sequential benchmark. For quicker experiments, use a smaller image named `example.jpg` or reduce `blur_radius` in the source.
- The benchmark does not compare the two output buffers for equality. Its JPEG is generated from the parallel buffer. Timing and speedup vary with hardware, thread count, compiler settings, and system load; more threads may not always help.

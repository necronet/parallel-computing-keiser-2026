#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void box_blur_sequential(const unsigned char *src, unsigned char *dst,
                         int width, int height, int channels, int radius)
{
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int count = 0;
            int channel_sums[4] = {0, 0, 0, 0};

            for (int dy = -radius; dy <= radius; ++dy) {
                int ny = y + dy;
                if (ny < 0 || ny >= height) continue;

                for (int dx = -radius; dx <= radius; ++dx) {
                    int nx = x + dx;
                    if (nx < 0 || nx >= width) continue;

                    int offset = (ny * width + nx) * channels;
                    for (int c = 0; c < channels; ++c) {
                        channel_sums[c] += src[offset + c];
                    }
                    count++;
                }
            }

            int target_offset = (y * width + x) * channels;
            for (int c = 0; c < channels; ++c) {
                dst[target_offset + c] = (unsigned char)(channel_sums[c] / count);
            }
        }
    }
}

// -----------------------------------------------------------------------------
// 2. Parallel Box Blur (OpenMP Multi-Threaded)
// -----------------------------------------------------------------------------
void box_blur_openmp(const unsigned char *src, unsigned char *dst,
                     int width, int height, int channels, int radius)
{
    #pragma omp parallel for schedule(static) default(none) \
        shared(src, dst, width, height, channels, radius)
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int count = 0;
            int channel_sums[4] = {0, 0, 0, 0};

            for (int dy = -radius; dy <= radius; ++dy) {
                int ny = y + dy;
                if (ny < 0 || ny >= height) continue;

                for (int dx = -radius; dx <= radius; ++dx) {
                    int nx = x + dx;
                    if (nx < 0 || nx >= width) continue;

                    int offset = (ny * width + nx) * channels;
                    for (int c = 0; c < channels; ++c) {
                        channel_sums[c] += src[offset + c];
                    }
                    count++;
                }
            }

            int target_offset = (y * width + x) * channels;
            for (int c = 0; c < channels; ++c) {
                dst[target_offset + c] = (unsigned char)(channel_sums[c] / count);
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Main Benchmark Driver
// -----------------------------------------------------------------------------
int main(void) {
    const char *input_path = "example.jpg";
    const int blur_radius = 15;     // 31x31 sampling kernel
    const int benchmark_runs = 3;   // Number of iterations to average

    int width = 0, height = 0, channels = 0;
    unsigned char *img_in = stbi_load(input_path, &width, &height, &channels, 0);
    if (!img_in) {
        fprintf(stderr, "Error: Could not load '%s'. Make sure the file exists.\n", input_path);
        return 1;
    }

    size_t img_size = (size_t)width * height * channels;
    unsigned char *img_seq = (unsigned char *)malloc(img_size);
    unsigned char *img_par = (unsigned char *)malloc(img_size);

    if (!img_seq || !img_par) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        stbi_image_free(img_in);
        free(img_seq);
        free(img_par);
        return 1;
    }

    printf("============================================================\n");
    printf("Image: %s (%dx%d, %d channels)\n", input_path, width, height, channels);
    printf("Blur Radius: %d (Kernel: %dx%d)\n", blur_radius, 2 * blur_radius + 1, 2 * blur_radius + 1);
    printf("Available OpenMP Threads: %d\n", omp_get_max_threads());
    printf("Benchmark runs per method: %d\n", benchmark_runs);
    printf("============================================================\n\n");

    // --- Benchmark Sequential ---
    printf("Running sequential version...\n");
    double seq_total_time = 0.0;
    for (int i = 0; i < benchmark_runs; ++i) {
        double t0 = omp_get_wtime();
        box_blur_sequential(img_in, img_seq, width, height, channels, blur_radius);
        double t1 = omp_get_wtime();
        seq_total_time += (t1 - t0);
    }
    double seq_avg_ms = (seq_total_time / benchmark_runs) * 1000.0;

    // --- Benchmark OpenMP ---
    printf("Running OpenMP version...\n");
    double par_total_time = 0.0;
    for (int i = 0; i < benchmark_runs; ++i) {
        double t0 = omp_get_wtime();
        box_blur_openmp(img_in, img_par, width, height, channels, blur_radius);
        double t1 = omp_get_wtime();
        par_total_time += (t1 - t0);
    }
    double par_avg_ms = (par_total_time / benchmark_runs) * 1000.0;

    // --- Print Results ---
    double speedup = seq_avg_ms / par_avg_ms;

    printf("\n--- Benchmark Results ---\n");
    printf("Sequential (1 thread):   %8.2f ms (avg)\n", seq_avg_ms);
    printf("OpenMP     (%d threads):  %8.2f ms (avg)\n", omp_get_max_threads(), par_avg_ms);
    printf("Speedup:                 %8.2fx\n", speedup);
    printf("Efficiency:              %8.2f%%\n", (speedup / omp_get_max_threads()) * 100.0);

    // Save parallel output to verify the blur was applied
    stbi_write_jpg("example_blurred.jpg", width, height, channels, img_par, 90);
    printf("\nSaved blurred output to 'example_blurred.jpg'\n");

    // Cleanup
    stbi_image_free(img_in);
    free(img_seq);
    free(img_par);

    return 0;
}

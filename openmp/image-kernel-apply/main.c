#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// Variable radius box blur. 
// A radius of 15 samples a (2*15 + 1) = 31x31 pixel window.
void box_blur_openmp(const unsigned char *src, unsigned char *dst, 
                    int width, int height, int channels, int radius) 
{
    #pragma omp parallel for schedule(static) default(none) \
        shared(src, dst, width, height, channels, radius)
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int count = 0;
            int channel_sums[4] = {0, 0, 0, 0};

            // Sample the window from [-radius, +radius]
            for (int dy = -radius; dy <= radius; ++dy) {
                int ny = y + dy;
                if (ny < 0 || ny >= height) continue;

                for (int dx = -radius; dx <= radius; ++dx) {
                    int nx = x + dx;
                    if (nx < 0 || nx >= width) continue;

                    int neighbor_offset = (ny * width + nx) * channels;
                    for (int c = 0; c < channels; ++c) {
                        channel_sums[c] += src[neighbor_offset + c];
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

int main(void) {
    const char *input_path = "example.jpg";
    const char *output_path = "example_blurred.jpg";

    int width = 0, height = 0, channels = 0;
    unsigned char *img_in = stbi_load(input_path, &width, &height, &channels, 0);
    if (!img_in) {
        fprintf(stderr, "Error: Could not load '%s'.\n", input_path);
        return 1;
    }

    size_t img_size = (size_t)width * height * channels;
    unsigned char *img_out = (unsigned char *)malloc(img_size);
    if (!img_out) {
        fprintf(stderr, "Error: Allocation failed.\n");
        stbi_image_free(img_in);
        return 1;
    }

    // Set radius:
    //  5  -> Mild blur (11x11 kernel)
    // 15  -> Clear, distinct blur (31x31 kernel)
    // 25+ -> Heavy blur (51x51 kernel)
    const int blur_radius = 15;

    printf("Processing %dx%d image (radius=%d, kernel=%dx%d)...\n", 
           width, height, blur_radius, 2 * blur_radius + 1, 2 * blur_radius + 1);

    double start_time = omp_get_wtime();

    box_blur_openmp(img_in, img_out, width, height, channels, blur_radius);

    double elapsed_ms = (omp_get_wtime() - start_time) * 1000.0;
    printf("Completed in %.2f ms using %d threads.\n", elapsed_ms, omp_get_max_threads());

    stbi_write_jpg(output_path, width, height, channels, img_out, 90);
    printf("Saved result to '%s'\n", output_path);

    stbi_image_free(img_in);
    free(img_out);
    return 0;
}

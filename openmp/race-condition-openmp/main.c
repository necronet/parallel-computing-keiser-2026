#include <stdio.h>

int counter = 0;  // Shared global variable

int main(void)
{
    #pragma omp parallel for num_threads(2)
    for (int i = 0; i < 2000000; i++) {
// Compiling this to  an  atomic CPU instructions
        #pragma omp atomic update
        counter++;
    }

    // OpenMP waits for all loop iterations to finish.
    printf("Counter: %d\n", counter);
    return 0;
}

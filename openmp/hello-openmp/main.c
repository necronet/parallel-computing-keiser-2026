#include <stdio.h>
#include <omp.h> // Required for OpenMP functions like omp_get_thread_num()

int main() {
    // 1. Define a parallel region
    #pragma omp parallel
    {
        // 2. Get the unique ID of the current thread
        int thread_id = omp_get_thread_num();
        
        // 3. Get the total number of threads in the team
        int total_threads = omp_get_num_threads();
        
        printf("Hello World from thread %d out of %d\n", thread_id, total_threads);
    } // Threads join back into the master thread here

    return 0;
}


# Hello, OpenMP

`#pragma omp parallel` starts a team of threads. Each thread calls `omp_get_thread_num()` for its ID and `omp_get_num_threads()` for the team size, then prints a greeting. This introduces the syllabus's OpenMP programming topic.

## Run

From this directory on Linux:

```sh
gcc -fopenmp main.c -o hello-openmp
OMP_NUM_THREADS=4 ./hello-openmp
```

For macOS build flags, see the [OpenMP setup guide](../README.MD). Expect four lines when the runtime can provide four threads, with IDs 0–3. Their order is unpredictable. Without `OMP_NUM_THREADS`, the runtime chooses a team size. The threads meet at the end of the parallel region before the program exits.

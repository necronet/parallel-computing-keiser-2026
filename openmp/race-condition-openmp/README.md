# OpenMP atomic counter

The filename refers to the race condition this example **prevents**. A `parallel for` distributes 2,000,000 loop iterations across a team explicitly set to two threads. Each iteration increments the same global `counter`, but `#pragma omp atomic update` ensures the increments are synchronized. The expected output is **`Counter: 2000000`**.

## Run

From this directory on Linux:

```sh
gcc -fopenmp main.c -o openmp-race-condition
./openmp-race-condition
```

For macOS build flags, see the [OpenMP setup guide](../README.MD). The existing `openmp_race_condition_atomic` file is a compiled macOS binary; build from `main.c` for your own machine. OpenMP waits for the loop's threads at its end before printing. Compare this atomic update with the [pthreads mutex](../../pthreads/pthreads-race-condition-mutexes/README.md): both protect shared state, although their APIs and costs differ.

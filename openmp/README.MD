# OpenMP examples

These examples support the syllabus's **shared-memory programming with OpenMP** topic (week 8). OpenMP uses compiler directives such as `#pragma omp parallel` and `#pragma omp parallel for` to assign work to threads. Build with OpenMP enabled; otherwise the directives may be ignored and programs that call `omp_*` functions may fail to link.

| Example | What it demonstrates |
| --- | --- |
| [Hello, OpenMP](hello-openmp/README.md) | Create a team of threads and query thread IDs. |
| [Atomic counter](race-condition-openmp/README.md) | Protect shared increments with `#pragma omp atomic update`. |
| [Image blur and benchmark](image-kernel-apply/README.md) | Distribute rows of a box blur and measure speedup. |

## Build on Linux

Install a GCC toolchain with OpenMP support. From an example directory, use `gcc -O2 -fopenmp main.c -o example`. The individual READMEs give exact commands; the image example also has a separate benchmark program.

## Build on macOS

Apple Clang does not accept `-fopenmp` on its own. Install the OpenMP runtime with `brew install libomp`, then point Clang at its headers and library. For example, from `openmp/hello-openmp`:

```sh
clang -O2 -Xpreprocessor -fopenmp \
  -I"$(brew --prefix libomp)/include" \
  -L"$(brew --prefix libomp)/lib" -lomp \
  main.c -o hello-openmp
./hello-openmp
```

Use the same flags for the other OpenMP source files, changing the source and output names. Run image programs from their own directory so they can find `example.jpg`.

## Thread count

Set `OMP_NUM_THREADS` before running an example, for example `OMP_NUM_THREADS=4 ./hello-openmp`. The atomic-counter example explicitly requests two threads with `num_threads(2)`, so this environment setting does not determine its team size. Output order may vary between runs.

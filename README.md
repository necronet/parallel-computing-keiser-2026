# Parallel computing examples

Small C programs for **CDA 4125: Concepts of Parallel and Distributed Processing** (Fall 2026). The [course syllabus](<Fall-2026-Term-CDA-4125-921175-Concepts-Of-Parallel-And-Distributed-Processing Fall 2026.pdf>) covers parallel programming platforms, simple parallel algorithms, and performance evaluation. These examples focus on **shared-memory programming**: POSIX threads (weeks 6–7) and OpenMP (week 8). The syllabus also covers MPI, CUDA, and other topics; there are no examples of those here yet.

## Examples

| Example | Main idea |
| --- | --- |
| [Hello, pthreads](pthreads/hello-pthreads/README.md) | Create a thread and wait for it with `pthread_join`. |
| [Pthreads data race](pthreads/pthreads-race-condition/README.md) | See what happens when two threads update a shared counter without synchronization. |
| [Pthreads mutex](pthreads/pthreads-race-condition-mutexes/README.md) | Protect the same counter with a mutex. |
| [Hello, OpenMP](openmp/hello-openmp/README.md) | Run a parallel region and identify its threads. |
| [OpenMP atomic counter](openmp/race-condition-openmp/README.md) | Synchronize updates to a shared counter with `atomic update`. |
| [Parallel image blur](openmp/image-kernel-apply/README.md) | Divide image rows among threads and compare sequential and parallel runtimes. |

Start with [pthreads](pthreads/README.md) to manage threads and synchronization explicitly, then [OpenMP](openmp/README.MD) to express parallel work with compiler directives.

## Requirements

- A C compiler with POSIX threads support for the pthreads examples.
- A C compiler with OpenMP support for the OpenMP examples. On Linux, GCC commonly provides both; compile with `-pthread` or `-fopenmp` as shown in each example. On macOS, Apple Clang needs a separate OpenMP runtime; see the [OpenMP setup guide](openmp/README.MD).

Run each command from the example's own directory. Each example README has its exact build and run commands. The image example uses the bundled `stb_image` headers and `example.jpg`; it needs no additional image library.

## What to observe

Thread output order can change from run to run. A data race makes the pthread counter result unreliable, even if it sometimes prints the expected value. Synchronization makes the counter correct, but can cost time. For the image benchmark, compare speedup across thread counts; faster execution is not guaranteed, and results depend on the machine and workload.

These programs demonstrate local shared memory. They do not communicate between machines.

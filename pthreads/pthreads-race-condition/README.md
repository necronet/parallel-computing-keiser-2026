# Pthreads data race

Two threads each run one million `counter++` operations on the same global integer. The program joins both workers before printing. The intended count is **2,000,000**, but `counter++` is a read, modification, and write that can overlap between threads. The resulting data race is **undefined behavior** in C: the printed value may be smaller, may appear correct, or may vary between runs. A particular result does not prove the code is safe.

## Run

From this directory:

```sh
cc -O0 -pthread main.c -o pthreads-race-condition
./pthreads-race-condition
```

`-O0` keeps the example easier to observe; it does not make the race valid. Try running it several times. Then compare it with the [mutex-protected version](../pthreads-race-condition-mutexes/README.md), which guarantees the expected count. This is an intentionally incorrect teaching example; do not use its shared-counter pattern in application code.

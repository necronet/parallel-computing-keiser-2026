# Pthreads counter with a mutex

This is the synchronized counterpart to the [data-race example](../pthreads-race-condition/README.md). Two workers each increment a shared counter one million times. `pthread_mutex_lock` and `pthread_mutex_unlock` surround each `counter++`, so only one thread changes it at a time. After both joins, the expected output is **`Counter: 2000000`**.

## Run

From this directory:

```sh
cc -pthread main.c -o pthreads-race-condition-mutexes
./pthreads-race-condition-mutexes
```

The mutex provides correctness, but locking on every increment adds synchronization overhead. This example is about the syllabus's shared-memory coordination topic, not a performance-optimized counter.

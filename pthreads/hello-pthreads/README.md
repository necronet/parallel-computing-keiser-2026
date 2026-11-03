# Hello, pthreads

The simplest example of the syllabus's Pthreads topic: the main thread creates one worker with `pthread_create`, waits for it with `pthread_join`, then prints its own message. Joining prevents the process from ending before the worker finishes.

## Run

From this directory:

```sh
cc -pthread main.c -o hello-pthreads
./hello-pthreads
```

You should see one line from the worker and one from the main thread. Because the main thread prints **after** the join, the worker's line comes first. The worker does not use an argument or return a value; both parameters are `NULL` in this introductory example.

# POSIX threads examples

These examples support the syllabus's **shared-memory programming with Pthreads** topic (weeks 6–7). POSIX threads let the program create and join threads directly. When threads share data, the programmer must coordinate access.

| Example | What it demonstrates |
| --- | --- |
| [Hello, pthreads](hello-pthreads/README.md) | `pthread_create` starts a worker; `pthread_join` waits for it. |
| [Data race](pthreads-race-condition/README.md) | Two workers increment one counter without synchronization. |
| [Mutex-protected counter](pthreads-race-condition-mutexes/README.md) | A mutex makes each increment exclusive. |

Use a C compiler with pthreads support. From an example directory, compile with `cc -pthread main.c -o example` and run `./example`. The individual READMEs provide commands and explain the output.

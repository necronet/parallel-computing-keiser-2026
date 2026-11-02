#include <pthread.h>
#include <stdio.h>

int counter = 0;  // Shared global variable

void *increment(void *arg)
{
    (void)arg;  // Unused

    for (int i = 0; i < 1000000; i++) {
        counter++;
    }

    return NULL;
}

int main(void)
{
    pthread_t thread1;
    pthread_t thread2;

    pthread_create(&thread1, NULL, increment, NULL);
    pthread_create(&thread2, NULL, increment, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Counter: %d\n", counter);
    return 0;
}

#include <stdio.h>
#include <pthread.h>

// Thread function must return void* and take void* as an argument
void* print_hello(void* arg) {
    printf("Hello World from the spawned thread!\n");
    return NULL;
}

int main() {
    pthread_t thread;

    // Create a new thread that executes print_hello
    if (pthread_create(&thread, NULL, print_hello, NULL) != 0) {
        perror("Failed to create thread");
        return 1;
    }

    // Wait for the thread to complete before exiting main
    pthread_join(thread, NULL);

    printf("Hello World from the main thread!\n");
    return 0;
}


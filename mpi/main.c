#include <mpi.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int rank, process_count;
    char greeting[100];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);

    if (rank == 0) {
        printf("Greetings from process 0 of %d!\n", process_count);

        for (int source = 1; source < process_count; source++) {
            MPI_Recv(greeting, sizeof(greeting), MPI_CHAR,
                     source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("%s\n", greeting);
        }
    } else {
        snprintf(greeting, sizeof(greeting),
                 "Greetings from process %d of %d!", rank, process_count);
        MPI_Send(greeting, 100, MPI_CHAR,
                 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}

#include <mpi.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int rank, size;
    char word[] = "Parallelization";
    int length = strlen(word);
    char letter, recv_letter;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank < length) {
        letter = word[rank];
    } else {
        letter = ' ';
    }

    printf("Initially: Process %d has '%c'\n", rank, letter);

    // Reverse distribution
    int target = length - rank - 1;
    if (target >= 0 && target < size) {
        MPI_Sendrecv(&letter, 1, MPI_CHAR, target, 0,
                     &recv_letter, 1, MPI_CHAR, target, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
        recv_letter = ' ';
    }

    MPI_Barrier(MPI_COMM_WORLD);
    printf("After reverse: Process %d has '%c'\n", rank, recv_letter);

    MPI_Finalize();
    return 0;
}

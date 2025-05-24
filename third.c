#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    MPI_Comm parent, intercomm;
    int rank, size, parent_rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_get_parent(&parent);

    if (parent == MPI_COMM_NULL) {
        int n;
        scanf("%d", &n);

        char *spawn_args[] = {NULL};
        MPI_Comm_spawn("./dynamic_spawn", spawn_args, n,
                       MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm,
                       MPI_ERRCODES_IGNORE);
    } else {
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_remote_size(parent, &size);
        MPI_Comm_rank(parent, &parent_rank); // often 0
        printf("I am %d process from %d processes! My parent is %d\n", rank, size + 1, parent_rank);
    }

    MPI_Finalize();
    return 0;
}

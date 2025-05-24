#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_N 10

int main(int argc, char *argv[]) {
    int rank, size, n;
    double A[MAX_N][MAX_N], B[MAX_N][MAX_N], C[MAX_N][MAX_N];
    double rowA[MAX_N], rowC[MAX_N];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        scanf("%d", &n);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                scanf("%lf", &A[i][j]);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                scanf("%lf", &B[i][j]);
    }

    // Broadcast n and B to all
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(B, MAX_N * MAX_N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Scatter rows of A
    MPI_Scatter(A, MAX_N, MPI_DOUBLE, rowA, MAX_N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Compute one row of C
    for (int j = 0; j < n; j++) {
        rowC[j] = 0;
        for (int k = 0; k < n; k++)
            rowC[j] += rowA[k] * B[k][j];
    }

    // Gather rows of C
    MPI_Gather(rowC, MAX_N, MPI_DOUBLE, C, MAX_N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                printf("%.0lf ", C[i][j]);
            printf("\n");
        }
    }

    MPI_Finalize();
    return 0;
}

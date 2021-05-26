#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define N_MATRIX 5
#define MATRIX_SIZE (N_MATRIX * N_MATRIX)
#define N_VECTOR N_MATRIX
#define N_PROC N_MATRIX

void multiply_matrix_slice_with_vector(
    int* matrix_row_start,
    int* vector_start,
    int* result_vector,
    int rank)
{
    int accumulator = 0;
    for (int i = 0; i < N_MATRIX; i++) {
        printf("%d: Calc: %d * %d\n", rank, matrix_row_start[i], vector_start[i]);
        accumulator += (matrix_row_start[i] * vector_start[i]);
    }
    *result_vector = accumulator;
}

void initialize_random_matrix_and_vector(
    int** matrix_to_initialize,
    int** vector_to_initialize)
{
    srand(time(0));
    // N_MATRIX * N_MATRIX
    // 5 4 3 2 5
    // 3 2 4 5 6
    // 5 2 6 4 2
    // 7 4 3 8 1
    // 1 2 4 5 6
    *matrix_to_initialize = (int*)malloc(sizeof(int) * N_MATRIX * N_MATRIX);
    printf("\n\nInitialized matrix:\n");
    for (int i = 0; i < MATRIX_SIZE; i++) {
        (*matrix_to_initialize)[i] = rand() % 9 + 1;
        printf("%d ", (*matrix_to_initialize)[i]);
        if (i % N_MATRIX == 4) {
            printf("\n");
        }
    }
    printf("\n\n");
    // N_VECTOR * 1
    // 5
    // 3
    // 6
    // 6
    // 7
    printf("\n\nInitialized vector:\n");
    for (int i = 0; i < N_VECTOR; i++) {
        (*vector_to_initialize)[i] = rand() % 9 + 1;
        printf("%d\n", (*vector_to_initialize)[i]);
    }
    printf("\n\n");
}

int main(int argc, char** argv)
{
    // MPI: init
    MPI_Init(&argc, &argv);
    // which rank am I?
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    printf("My rank: %d\n", my_rank);
    // only root process should has a exact copy of the matrix
    int* matrix = (int*)NULL;
    // every process needs a vector
    int* vector = (int*)malloc(sizeof(int) * N_VECTOR);
    // if you are root process, you should initialize all the things.
    if (my_rank == 0){
        initialize_random_matrix_and_vector(&matrix, &vector);
    }
    int* result_vector = (int*)malloc(sizeof(int) * N_VECTOR);
    int* result_vector_buffer = (int*)malloc(sizeof(int));
    int* received_matrix_slice = (int*)malloc(sizeof(int) * N_MATRIX);

    // MPI: barrier
    MPI_Barrier(MPI_COMM_WORLD);

    // from initialized matrix, sends slice with size N_MATRIX to each process
    MPI_Scatter(
        matrix, N_MATRIX, MPI_INT,
        received_matrix_slice, N_MATRIX, MPI_INT,
        0,
        MPI_COMM_WORLD);
    // from initialized vector, broadcast size N_VECTOR to each process

    MPI_Bcast(vector, N_VECTOR, MPI_INT, 0, MPI_COMM_WORLD);

    // real workload
    multiply_matrix_slice_with_vector(
        received_matrix_slice,
        vector,
        result_vector_buffer,
        my_rank);

    // MPI: barrier
    MPI_Barrier(MPI_COMM_WORLD);

    // gather result from other process
    MPI_Gather(
        result_vector_buffer, 1, MPI_INT,
        result_vector, 1, MPI_INT,
        0,
        MPI_COMM_WORLD);

    // root collects and show the result
    if (my_rank == 0) {
        // 102
        // 117
        // 105
        // 120
        // 107
        printf("\n\nResult:\n");
        for (int i = 0; i < N_VECTOR; i++) {
            printf("%d\n", result_vector[i]);
        }
    }

    // MPI: finalize
    MPI_Finalize();

    // cleanup
    if (my_rank == 0) {
        free(matrix);
    }
    free(vector);
    free(result_vector);
    free(result_vector_buffer);
    free(received_matrix_slice);
    return 0;
}

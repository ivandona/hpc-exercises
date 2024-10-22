#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mpi.h>

const int MAX_STRING = 100;

int main(void) {
    int local_value;
    int total_value;
    int source;
    int dest;
    int max = 10;
    int min = 1;
    int comm_sz;
    int my_rank;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    local_value = rand() % (max - min + 1) + min;

    printf("Process %d with value %d\n", my_rank, local_value);

    if(my_rank != 0) {
        MPI_Send(&local_value, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(&total_value, 1, MPI_DOUBLE, 0, 0, 
            MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
        total_value = local_value;
        for(source = 1; source < comm_sz; source++) {
            MPI_Recv(&local_value, 1, MPI_DOUBLE, source, 0, 
                MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_value += local_value;
        }

        for(dest = 1; dest < comm_sz; dest++) {
            MPI_Send(&total_value, 1, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
        }
    }

    local_value = total_value;

    printf("Process %d with new value %d\n", my_rank, local_value);

    MPI_Finalize();
    return 0;

}
#include <stdio.h>
#include <string.h>
#include <mpi.h>

const int MAX_STRING = 100;

int main(void) {
    char greeting[MAX_STRING];
    int comm_sz;
    int my_rank;
    int next_rank; //rank for send
    int previous_rank; //rank for receive

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    next_rank = (my_rank + 1) % comm_sz;

    previous_rank = (my_rank + comm_sz -1) % comm_sz;

    sprintf(greeting, "Greetings from process %d of %d!", my_rank+1, comm_sz);
    MPI_Send(greeting, strlen(greeting)+1, MPI_CHAR, next_rank, 0, MPI_COMM_WORLD);
    MPI_Recv(greeting, MAX_STRING, MPI_CHAR, previous_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("%s\n", greeting);
    MPI_Finalize();
    return 0;

}
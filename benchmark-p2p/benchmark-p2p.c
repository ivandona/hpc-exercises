#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <mpi.h>

const int MAX_BYTES = 1048576;
const int N_TEST_LATENCY = 100;
const int N_TEST_BANDWIDTH = 10;
const int MAX_POWER_OF = 21;

double compute_avg_latency(int my_rank) {
    int i;
    double total_time, avg_latency;
    char test = NULL;
    struct timeval start, end;

    gettimeofday(&start, NULL);

    for(i = 0; i < N_TEST_LATENCY; i++) {        
        if(my_rank == 0) {
            MPI_Send(&test, 0, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&test, 0, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        } else {
            MPI_Recv(&test, 0, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(&test, 0, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        }
    }
    
    gettimeofday(&end, NULL);

    total_time = ((end.tv_sec * 1000000 + end.tv_usec)
        - (start.tv_sec * 1000000 + start.tv_usec)) / 2;

    avg_latency = total_time / N_TEST_LATENCY;

    return avg_latency;
}

void compute_avg_bandwidth(int my_rank, double avg_latency) {
    int i, j;
    char *data;
    long int bytes;
    double total_time, actual_time, actual_time_sec;
    float avg_bandwidth;
    struct timeval start, end;

    bytes = MAX_BYTES * sizeof(char);
    data = (char *) malloc(bytes);

    for (i = 0; i < bytes; i++) {
        data[i] = 'a';
    }

    for(i = 0; i < MAX_POWER_OF; i++) {
        bytes = pow(2, i) * sizeof(char);

        gettimeofday(&start, NULL);

        for(j = 0; j < N_TEST_BANDWIDTH; j++) {
            if(my_rank == 0) {               
                MPI_Send(data, bytes, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(data, 0, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            } else {
                MPI_Recv(data, bytes, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(data, 0, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
            }    
        }

        gettimeofday(&end, NULL);

        total_time = (end.tv_sec * 1000000 + end.tv_usec)
                            - (start.tv_sec * 1000000 + start.tv_usec);
        actual_time = total_time - avg_latency;
        actual_time_sec = actual_time / 1000;
        avg_bandwidth = bytes / actual_time_sec / 1000;

        if(my_rank == 0) {
            printf("n: %ld, time (sec): %f, rate (MB/sec): %f\n", bytes, actual_time_sec, avg_bandwidth);
        }
    }

    free(data);
}

int main(void) {
    int comm_sz;
    int my_rank;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    double avg_latency = compute_avg_latency(my_rank);

    compute_avg_bandwidth(my_rank, avg_latency);

    MPI_Finalize();
    return 0;

}
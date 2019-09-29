#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <stddef.h>

typedef struct car_s {
        int shifts;
        int topSpeed;
} car;

int main(int argc, char **argv) {

    const int tag = 13;
    int size, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        fprintf(stderr,"Requires at least two processes.\n");
        exit(-1);
    }

    /* create a type for struct car */
    const int nitems=2;
    int          blocklengths[2] = {1,1};
    MPI_Datatype types[2] = {MPI_INT, MPI_INT};
    MPI_Datatype mpi_car_type;
    MPI_Aint     offsets[2];

    offsets[0] = offsetof(car, shifts);
    offsets[1] = offsetof(car, topSpeed);

    MPI_Type_create_struct(nitems, blocklengths, offsets, types, &mpi_car_type);
    MPI_Type_commit(&mpi_car_type);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        car send[2];
        send[0].shifts = 4;
        send[0].topSpeed = 100;
        send[1].shifts = 40;
        send[1].topSpeed = 10;


        const int dest = 1;
        MPI_Send(&send,   2, mpi_car_type, dest, tag, MPI_COMM_WORLD);

        printf("Rank %d: sent structure car\n", rank);
    }
    if (rank == 1) {
        MPI_Status status;
        const int src=0;

        car recv[2];

        MPI_Recv(&recv,   2, mpi_car_type, src, tag, MPI_COMM_WORLD, &status);
        printf("Rank %d: Received: shifts = %d topSpeed = %d\n", rank,
                 recv[0].shifts, recv[1].topSpeed);
    }

    MPI_Type_free(&mpi_car_type);
    MPI_Finalize();

    return 0;
}

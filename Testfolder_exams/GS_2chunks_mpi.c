#include <stdio.h>
#include <mpi.h>

void GS_iteration_2_chunks_mpi(int my_rank, int kmax, int my_jmax, int imax, double ***my_phi) {
   
    int i, j, k;
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    int send_idx = my_rank * (1 - (my_jmax - 2)) + (my_jmax - 2);
    int send_dest = (my_rank == 0) ? 1 : 0; // do something more fancy
    int recv_idx = (my_rank == 0) ? my_jmax - 1 : 0; // do something more fancy
    int recv_dest = (my_rank == 0) ? 1 : 0; // do something more fancy

    for (int m = 1; m < kmax + size-2; m++){
        k = m - my_rank;
        if (k > 0 && k < kmax - 1){
            //communication
            if (my_rank == 1){
                MPI_Recv(my_phi[k][recv_idx], imax, MPI_DOUBLE, recv_dest, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            if (my_rank == 0 && k > 1){
                MPI_Recv(my_phi[k-1][recv_idx], imax, MPI_DOUBLE, recv_dest, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            
            for (j = 1; j < my_jmax -1; j++) {
                for (i = 1;i < imax -1 ; i++) {
                    // Perform local computation
                    my_phi[k][j][i] = ( my_phi[k-1][j][i] + my_phi[k][j-1][i]
                                        +my_phi[k][j][i-1] + my_phi[k][j][i+1]
                                        +my_phi[k][j+1][i] + my_phi[k+1][j][i])/6.0;
                }
            }

            // communication
            if (my_rank == 0){
                MPI_Send(my_phi[k][send_idx],imax, MPI_DOUBLE, send_dest, 1, MPI_COMM_WORLD);
            }
            if (my_rank == 1 && k < kmax - 2){
                MPI_Send(my_phi[k][send_idx], imax, MPI_DOUBLE, send_dest, 2 , MPI_COMM_WORLD);
            }
        }
        // printf("Rank %d, k = %d, m = %d\n", my_rank, k, m);
    }
    // Send the last boundary condition
    if (my_rank == 1) {
        MPI_Send(my_phi[kmax - 2][send_idx], imax, MPI_DOUBLE, send_dest, 3, MPI_COMM_WORLD);
    }
    if (my_rank == 0) {
        MPI_Recv(my_phi[kmax - 2][recv_idx], imax, MPI_DOUBLE, recv_dest, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

}




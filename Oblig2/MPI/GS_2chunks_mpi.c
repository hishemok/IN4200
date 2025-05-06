#include <stdio.h>
#include <mpi.h>

void GS_iteration_2_chunks_mpi(int my_rank, int kmax, int my_jmax, int imax, double ***my_phi) {

    int i,j,k;    
    int tag = 1;
    int destination = (my_rank == 0) ? 1 : 0;
    int source = (my_rank == 0) ? 1 : 0;
    int count = imax;
    MPI_Request reqs[2];

    // first wavefront: only computation on left chunck at level k=1
    if (my_rank == 0){
        for (j = 1; j < my_jmax-1; j++){
            for (i = 1; i < imax-1; i++){
                my_phi[1][j][i] = ( my_phi[0][j][i] + my_phi[1][j-1][i]
                                   +my_phi[1][j][i-1] + my_phi[1][j][i+1]
                                   +my_phi[1][j+1][i] + my_phi[2][j][i])/6.0;
            }
        }

        MPI_Send(my_phi[1][my_jmax - 2], count, MPI_DOUBLE, destination, tag, MPI_COMM_WORLD);        
    }
    // rank 1 receives k=1 from rank 0
    if (my_rank == 1){
        MPI_Recv(my_phi[1][0], count, MPI_DOUBLE, source, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    //Main Bulk
    for (k = 2; k <= kmax-2; k++){        
        if (my_rank == 0){
            
            tag = 2;
            MPI_Irecv(my_phi[k-1][my_jmax - 1], count, MPI_DOUBLE, source, tag, MPI_COMM_WORLD, &reqs[0]);

            for (j = 1; j < my_jmax-1; j++){               
                for (i = 1; i < imax-1; i++){
                    my_phi[k][j][i] = ( my_phi[k-1][j][i] + my_phi[k][j-1][i]
                                       +my_phi[k][j][i-1] + my_phi[k][j][i+1]
                                       +my_phi[k][j+1][i] + my_phi[k+1][j][i])/6.0;
                }
            }
            tag = 3;
            MPI_Isend(my_phi[k][my_jmax - 2], count, MPI_DOUBLE, destination, tag, MPI_COMM_WORLD,&reqs[1]);
             
        }        
        if (my_rank == 1){
            for (j = 1; j < my_jmax-1; j++){
                for (i = 1; i < imax-1; i++){
                    my_phi[k-1][j][i] = ( my_phi[k-2][j][i] + my_phi[k-1][j-1][i]
                                         +my_phi[k-1][j][i-1] + my_phi[k-1][j][i+1]
                                         +my_phi[k-1][j+1][i] + my_phi[k][j][i])/6.0;

                }
            }
            tag = 2;            
            MPI_Isend(my_phi[k-1][1], count, MPI_DOUBLE, destination, tag, MPI_COMM_WORLD,&reqs[1]);

            tag = 3;
            MPI_Irecv(my_phi[k][0], count, MPI_DOUBLE, source, tag, MPI_COMM_WORLD,&reqs[0]);
        }
        MPI_Waitall(2, reqs, MPI_STATUS_IGNORE);
    }

    // Last wavefront: only computation on right chunk at level k=kmax-2
    // rank 0 receives kmax-2 from rank 1
    tag = 4;
    if (my_rank == 1){
        for (j = 1; j < my_jmax-1; j++){
            for (i = 1; i < imax-1; i++){
                my_phi[kmax-2][j][i] = ( my_phi[kmax-3][j][i] + my_phi[kmax-2][j-1][i]
                                        +my_phi[kmax-2][j][i-1] + my_phi[kmax-2][j][i+1]
                                        +my_phi[kmax-2][j+1][i] + my_phi[kmax-1][j][i])/6.0;
            }
        }
        MPI_Send(my_phi[kmax-2][1], count, MPI_DOUBLE, destination, tag, MPI_COMM_WORLD);

    }
    if (my_rank == 0) {
        // receive kmax-2 from rank 1
            MPI_Recv(my_phi[kmax-2][my_jmax-1], count, MPI_DOUBLE, source, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

}

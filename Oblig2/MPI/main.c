#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include "functions.h"

int main(int nargs, char **args) {


    MPI_Init(&nargs, &args);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int num_iter, kmax, jmax, imax;

    if (rank == 0){
        printf("\n____________________________________\n");

        if (nargs != 5){
            printf("Usage: %s <num_iter> <kmax> <jmax> <imax>  \n", args[0]);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        num_iter = atoi(args[1]);
        kmax = atoi(args[2]);
        jmax = atoi(args[3]);
        imax = atoi(args[4]);

        if (num_iter <= 0 || kmax <= 0 || jmax <= 0 || imax <= 0) {
            printf("Error: All dimensions must be greater than 0.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);  
        }
    }

    //Broadcast params to rank1
    int root = 0;
    int count = 1;
    MPI_Bcast(&num_iter, count, MPI_INT, root, MPI_COMM_WORLD);
    MPI_Bcast(&kmax, count, MPI_INT, root, MPI_COMM_WORLD);
    MPI_Bcast(&jmax, count, MPI_INT, root, MPI_COMM_WORLD);
    MPI_Bcast(&imax, count, MPI_INT, root, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    printf("Rank %d: num_iter = %d, kmax = %d, jmax = %d, imax = %d\n", rank, num_iter, kmax, jmax, imax);


    // create 3D arrays
    double ***my_array; //Global array
    double ***my_local_array; //Local array for each rank
    double my_jmax = jmax/2 + 1;
    // Allocate memory for the 3D arrays
    allocate_array3D(kmax, jmax, imax, &my_array); // FULL array
    allocate_array3D(kmax, my_jmax, imax, &my_local_array); // Local array for each rank
 

    for (int k = 0; k < kmax; k++) {
        for (int j = 0; j < jmax; j++) {
            for (int i = 0; i < imax; i++) {
                double value = (double)( k * (k + 1) + (j - 1) * (i + 2) ); // Some random varying value
                my_array[k][j][i] = value;
                
                if (rank == 0 && j < my_jmax){ // First half goes to rank 0
                    my_local_array[k][j][i] = value;
                }
                
                if (rank == 1 && j >= my_jmax-2){ // Second half goes to rank 1
                    int j_shifted = j - my_jmax + 2; // Shift j-index for rank 1
                    my_local_array[k][j_shifted][i] = value;
                }

            }
        }
    }


  //Running iterations
    for (int i = 0; i < num_iter; i++){    
        GS_iteration_2_chunks(kmax, jmax, imax, my_array); 
        GS_iteration_2_chunks_mpi(rank, kmax, my_jmax, imax, my_local_array);
    }



    if (rank == 0) {
        int tag; //Create tag variable
        int destination = 1; // Destination rank
        int count = imax; // Number of elements to send


        // Initalize the global array
        double ***global_phi = NULL;
        double ***incoming_array = NULL;

        allocate_array3D(kmax, jmax, imax, &global_phi);
        allocate_array3D(kmax, my_jmax, imax, &incoming_array);
 
    
        // Now receive the second half from rank 1
        for (int k = 0; k < kmax; k++) {
            for (int j = 0; j < my_jmax; j++) {
                tag = k + j;
                MPI_Recv(incoming_array[k][j], count, MPI_DOUBLE, destination, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }

        // Combine the two halves into the global array
        for (int k = 0; k < kmax; k++) {
            for (int j = 0; j < jmax; j++) {
                for (int i = 0; i < imax; i++) {
                    if (j < my_jmax) {
                        global_phi[k][j][i] = my_local_array[k][j][i];
                    } else {
                        int j_shifted = j - my_jmax + 2; // Shift j-index for rank 1
                        global_phi[k][j][i] = incoming_array[k][j_shifted][i];
                    }
                }
            }
        }


        // Print the global array
        double distance = euclidean_distance(kmax, jmax, imax, my_array, global_phi);
        printf("num iters=%d, kmax=%d, jmax=%d, imax=%d, diff=%g\n",
            num_iter, kmax, jmax, imax, distance);

        // Free the allocated memory
        for (int k = 0; k < kmax; k++) {
            for (int j = 0; j < jmax; j++) {
                free(global_phi[k][j]);
            }
            free(global_phi[k]);
        }
        free(global_phi);
        for (int k = 0; k < kmax; k++) {
            for (int j = 0; j < my_jmax; j++) {
                free(incoming_array[k][j]);
            }
            free(incoming_array[k]);
        }
        free(incoming_array);
    
    }
    
    if (rank == 1) {
        // Send the second half of the local array to rank 0
        int tag;
        int destination = 0;
        int count = imax;

        for (int k = 0; k < kmax; k++) {
            for (int j = 0; j < my_jmax; j++) {  
                tag = k + j;
                MPI_Send(my_local_array[k][j], count, MPI_DOUBLE, destination, tag, MPI_COMM_WORLD);
            }
        }

    }
    // Free the allocated memory

    for (int k = 0; k < kmax; k++) {
        for (int j = 0; j < jmax; j++) {
            free(my_array[k][j]);
        }
        free(my_array[k]);
    }
    free(my_array);


    for (int k = 0; k < kmax; k++) {
        for (int j = 0; j < my_jmax; j++) {

            free(my_local_array[k][j]);
        }
        free(my_local_array[k]);
    }
    
    free(my_local_array);

    MPI_Finalize();
    return 0;
}


# Parallelizing the Gauss-Seidel Algorithm by MPI


## Structure
To main folders:

```
IN4200_Oblig2_hishemok/
├── MPI/
│   ├── allocate_3d.c
│   ├── euclidian_distance.c
│   ├── functions.h
│   ├── GS_2chunks_mpi.c
│   ├── GS_2chunks.c
│   ├── GS.c
│   └── main.c
├── Serial/
│   ├── allocate_3d.c
│   ├── euclidian_distance.c
│   ├── functions.h
│   ├── GS_2chunks.c
│   ├── GS.c
│   └── main.c
├── Makefile
└── README.md
```

## Compilation and running

Type make in the command line to compile all files in both serial and MPI folders
```
make
```

To run a default parametrized serial version, type:
```
make serial_run
```


To run a default parametrized MPI version, type:
```
make mpi_run
```

To clean out the object files, run:
```
make clean
```

__In order to change the input parameters for the serial version, type__
```
make && ./serial_program  num_iters kmax jmax imax
```
__And for the MPI version__

```
make && mpirun -np 2 ./mpi_program num_iters kmax jmax imax
```

## Example runs
### Serial
INN:
```
make && ./serial_program  100 20 30 40
```

OUT:
```
gcc -Wall -Wextra -I./include -lm -o serial_program Serial/allocate_3d.c Serial/euclidian_distance.c Serial/GS_2chunks.c Serial/GS.c Serial/main.c
mpicc -Wall -Wextra -I./include -lm -o mpi_program MPI/allocate_3d.c MPI/euclidian_distance.c MPI/GS_2chunks.c MPI/GS_2chunks_mpi.c MPI/GS.c MPI/main.c

____________________________________
num iters=100, kmax=20, jmax=30, imax=40, diff=0
 ```

### MPI
INN:
```
make && mpirun -np 2 ./mpi_program 100 20 30 20
```

OUT:
``` 
gcc -Wall -Wextra -I./include -lm -o serial_program Serial/allocate_3d.c Serial/euclidian_distance.c Serial/GS_2chunks.c Serial/GS.c Serial/main.c
mpicc -Wall -Wextra -I./include -lm -o mpi_program MPI/allocate_3d.c MPI/euclidian_distance.c MPI/GS_2chunks.c MPI/GS_2chunks_mpi.c MPI/GS.c MPI/main.c

____________________________________
Rank 1: num_iter = 100, kmax = 20, jmax = 30, imax = 20
Rank 0: num_iter = 100, kmax = 20, jmax = 30, imax = 20
num iters=100, kmax=20, jmax=30, imax=20, diff=0

```
CC = gcc
CC_PARALLEL = mpicc
CFLAGS = -Wall -Wextra -I./include -lm

SERIAL_DIR = Serial
MPI_DIR = MPI

SERIAL_SRCS=$(wildcard $(SERIAL_DIR)/*.c)
MPI_SRCS=$(wildcard $(MPI_DIR)/*.c)

SERIAL_BIN=serial_program
MPI_BIN=mpi_program

all: serial mpi

serial: $(SERIAL_SRCS)
	$(CC) $(CFLAGS) -o $(SERIAL_BIN) $(SERIAL_SRCS)

mpi: $(MPI_SRCS)
	$(CC_PARALLEL) $(CFLAGS) -o $(MPI_BIN) $(MPI_SRCS)

# Default run parameters:
N = 10000
N_k = 20
N_j = 30
N_i = 20


run_serial: serial
	./$(SERIAL_BIN) $(N) $(N_k) $(N_j) $(N_i)
run_mpi: mpi
	mpirun -np 2 ./$(MPI_BIN) $(N) $(N_k) $(N_j) $(N_i)
clean:
	rm -f $(SERIAL_BIN) $(MPI_BIN)


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


serial_run: serial
	./$(SERIAL_BIN) 100 100 500 100
mpi_run: mpi
	mpirun -np 2 ./$(MPI_BIN) 100 100 500 100
clean:
	rm -f $(SERIAL_BIN) $(MPI_BIN)


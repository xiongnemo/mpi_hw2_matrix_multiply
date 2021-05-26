hw2_matrix_multiply.o:  hw2_matrix_multiply.c
                        mpicxx -g -Wall -o hw2_matrix_multiply.o ./hw2_matrix_multiply.c

run:    hw2_matrix_multiply.o
        mpirun -n 5 ./hw2_matrix_multiply.o

clean:
        rm hw2_matrix_multiply.o

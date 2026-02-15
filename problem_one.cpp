#include "mpi.h"
#include <iostream>

int main(int argc, char *argv[]){
    int size, rank;
    MPI_Init(&argc,&argv);
    
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    std::cout << "Hello from rank: " << rank << " and size: " << size << std::endl;
    
    MPI_Finalize();
    return 0;
}
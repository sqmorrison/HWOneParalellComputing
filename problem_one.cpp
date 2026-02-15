#include "mpi.h"
#include <iostream>

int main(int argc, char *argv[]){
    int size, rank, sent_int = 0;
    MPI_Init(&argc,&argv);
    
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    if(rank == 0){
        //send to next node
        std::cout << "Sending initilized with index: " << sent_int << std::endl;
        MPI_Send(&sent_int, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        //recieve from last node
        MPI_Recv(&sent_int, 1, MPI_INT, (size - 1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Last Recieved Index: " << sent_int << std::endl;
    }
    else {
        //recieve from left neighbor
        MPI_Recv(&sent_int, 1, MPI_INT, (rank - 1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        sent_int++;
        std::cout << "Recieved int: " << sent_int << " on rank: " << rank << std::endl;
        
        //if last process, send to 0, otherwise end to right neighbor
        if(rank == size - 1){
            //send to start process
            std::cout << "sending back to process 0: " << sent_int << std::endl;
            MPI_Send(&sent_int, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
        else{
            MPI_Send(&sent_int, 1, MPI_INT, (rank + 1), 0, MPI_COMM_WORLD);
        }
    }
    
    MPI_Finalize();
    return 0;
}
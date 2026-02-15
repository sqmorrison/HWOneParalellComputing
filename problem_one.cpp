#include "mpi.h"
#include <iostream>

int main(int argc, char *argv[]){
    int size, rank, n, sent_int = 0;
    MPI_Init(&argc,&argv);
    
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    n = atoi(argv[1]);
    
    for(int i = 0; i < n; i++){
        if(rank == 0){
            if(i > 1){
                sent_int++;
            }
            //send to next node
            std::cout << "Sending from process 0 with int: " << sent_int << std::endl;
            MPI_Send(&sent_int, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            //recieve from last node
            MPI_Recv(&sent_int, 1, MPI_INT, (size - 1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "end of loop: " << i + 1 << " we get int: " << sent_int << std::endl;
        }
        else {
            //recieve from left neighbor
            MPI_Recv(&sent_int, 1, MPI_INT, (rank - 1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "Recieved int: " << sent_int << " on rank: " << rank << std::endl;
            sent_int++;
            
            //if last process, send to 0, otherwise end to right neighbor
            if(rank == size - 1){
                //send to start process
                std::cout << "sending back to process 0: " << sent_int << std::endl;
                MPI_Send(&sent_int, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            }
            else{
                //send to right neighbor
                MPI_Send(&sent_int, 1, MPI_INT, (rank + 1), 0, MPI_COMM_WORLD);
            }
        }
    }
    
    MPI_Finalize();
    return 0;
}
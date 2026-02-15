#include <iostream>
#include <cstdlib>
#include <mpi.h>

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Check for correct number of command line arguments
    if (argc != 2) {
        if (rank == 0) {
            std::cerr << "Usage: mpirun -np <num_procs> " << argv[0] << " <number_of_loops>\n";
        }
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    int num_loops = std::atoi(argv[1]);
    int token = 0;

    if (num_loops <= 0) {
        if (rank == 0) {
            std::cerr << "Please provide a loop count greater than 0.\n";
        }
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    // Process 0 logic
    if (rank == 0) {
        for (int i = 0; i < num_loops; i++) {
            if (i > 0) {
                // On subsequent loops, receive from the last process and increment
                MPI_Recv(&token, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                token++;
            }
            
            // Send to process 1
            // On the first loop (i == 0), the token is 0 and is NOT incremented yet
            MPI_Send(&token, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            std::cout << "Process 0 sent token " << token << " to Process 1 (Loop " << i + 1 << ")" << std::endl;
        }

        // Final receive to complete the last loop's ring
        MPI_Recv(&token, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "\n--- Final Result ---\n";
        std::cout << "Process 0 received final token value: " << token << " after " << num_loops << " loops." << std::endl;

    } 
    // All other processes logic
    else {
        int next_rank = (rank + 1) % size; // Next process in the ring, wraps around to 0
        int prev_rank = rank - 1;

        for (int i = 0; i < num_loops; i++) {
            // Receive from the previous process
            MPI_Recv(&token, 1, MPI_INT, prev_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            // Increment the token
            token++;
            
            // Send to the next process
            MPI_Send(&token, 1, MPI_INT, next_rank, 0, MPI_COMM_WORLD);
        }
    }

    // Clean up and exit
    MPI_Finalize();
    return EXIT_SUCCESS;
}

/* Gemini 3 was used in the generation of this code, no extra configuration was made, using the Pro model
 * Prompt 1: Build me a cpp file utilizing openMPI that does the following: passes and integer from
 *           process zero to process one to process two, etc in a loop and then back to process 0 
 *           terminating after n times (speicified in the command line). An example of this would be round robin
 *           the first process should not increment the integer on the first loop, but should increment on subsequent loops
 *           
 * Analysis: the code generated was given after 1 round of prompting, no extra prompts were needed.
 *           Gemini wrote this code in a cleaner, more efficent way than I did the first time I wrote this program.
 *           it also included things like command line input handling, although our ideas of how to accomplish this task were the same.
 */
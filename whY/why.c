#include <stdio.h>
#include <stdlib.h>
#include <math.h>	
#include <string.h>	
#include <mpi.h>	
#include <unistd.h>	


int main(int argc, char ** argv){
  /*******************************************************************/
  /* 1. Initialization */
  /*******************************************************************/
	printf("hello");
  int myproc, nprocs, len;
  MPI_Init(&argc, &argv);
	printf("hello 2");
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myproc);
  MPI_Request request[1000];
  MPI_Status status;

	MPI_Finalize();


	return 0;
}

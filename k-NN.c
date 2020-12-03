#include <stdio.h>
#include <stdlib.h>
#include <math.h>	// To use the sqrt function
#include <string.h>	// To use the memcpy function
// #include <time.h>	// To seed the randomizer

#include <mpi.h>	// Main parallel library
#include <unistd.h>	// Needed for gethostname and sleep

typedef struct {
  int attr[4]; // sepal_l, sepal_w, petal_l, petal_w;
	char class[20];
} plant;

float dist(plant a, plant b){
	return (float)1.0;
}

void kNN(int myproc, int nprocs, int data_size){
  printf("myproc: %d nprocs: %d data_size: %d\n", myproc, nprocs, data_size);
}


int main(int argc, char ** argv){
  /*******************************************************************/
  /* Initialization */
  /*******************************************************************/
  int myproc, nprocs, len;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myproc);

	char *buf = NULL; 
  size_t leng; float a, b, c, d; char ch;
  plant coll[100];
  int plant_idx=0;

  /*******************************************************************/
  /* read data input */
  /*******************************************************************/
  if(myproc==0){
    FILE *fp = fopen(argv[1],"r");
  	while(getline(&buf, &leng, fp)>0){
  		sscanf(buf, "%f %[,] %f %[,] %f %[,] %f", &a, &ch, &b, &ch, &c, &ch, &d);
  		printf("%f %f %f %f\n", a, b, c, d);
      coll[plant_idx].attr[0]=a;
      coll[plant_idx].attr[1]=b;
      coll[plant_idx].attr[2]=c;
      coll[plant_idx].attr[3]=d;
      plant_idx++;
      printf("plant_idx: %d\n",plant_idx);
      //collection[plant_idx].class=____;
  	}
  }
  MPI_Barrier(MPI_COMM_WORLD);
  /*******************************************************************/
  /* 1. send data_size to all procs with rank greater than 0 */
  /*******************************************************************/
  int data_size;
  MPI_Request request[100];
  MPI_Status status;
  if(myproc==0){
    for(int i=1;i<nprocs;i++){
      MPI_Isend(&plant_idx, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &request[i]);
      MPI_Wait(&request[i], &status);
    }
  } else {
    MPI_Irecv(&data_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request[0]);
    MPI_Wait(&request[0], &status);
    printf("\t\tdata_size: %d\n", data_size);
  }
  MPI_Barrier(MPI_COMM_WORLD);

  /*******************************************************************/
  /* */
  /*******************************************************************/
  if(myproc>0){
    kNN(myproc, nprocs, plant_idx);
  }



  MPI_Finalize();
}


/*********************************************************************/
// TODO
/*

  1. send data_size to all procs
  2. if myproc>0, call kNN()
  3. 



int MPI_Isend(
  void *buf,
  int count,
  MPI_Datatype datatype,
  int dest,
  int tag,
  MPI_Comm comm,
  MPI_Request *request
);







*/













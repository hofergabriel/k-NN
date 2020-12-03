#include <stdio.h>
#include <stdlib.h>
#include <math.h>	// To use the sqrt function
#include <string.h>	// To use the memcpy function
// #include <time.h>	// To seed the randomizer

#include <mpi.h>	// Main parallel library
#include <unistd.h>	// Needed for gethostname and sleep

typedef struct {
  int sepal_l, sepal_w, petal_l, petal_w;
	char class[20];
} plant;

float dist(plant a, plant b){
	return (float)1.0;
}

int main(int argc, char ** argv){


  /******************************************************/
  int    myproc, nprocs, len;
  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myproc);



  printf("hellw\n");
  if(myproc==0){
    printf("proc 0 a\n");
  	char *buf = NULL;
  	size_t leng;
  	float a, b, c, d;
  	char ch;

    FILE *fp = fopen(argv[1],"r");
  	while(getline(&buf, &leng, fp)>0){
  		getline(&buf, &leng, fp);
  		sscanf(buf, "%f %[,] %f %[,] %f %[,] %f", &a, &ch, &b, &ch, &c, &ch, &d);
  		printf("%f %f %f %f\n", a, b, c, d);
  	}
  }

  printf("rank: %d\n", myproc);



  MPI_Finalize();
}







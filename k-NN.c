/*******************************************************************//*
Author: Gabriel Hofer
Date: December 7, 2020
Course: CSC-410

What is the difference between MPI_Barrier and MPI_Wait?
*//*******************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>	
#include <string.h>	
#include <mpi.h>	
#include <unistd.h>	

typedef struct {
  float attr[4]; // sepal_l, sepal_w, petal_l, petal_w;
  char class[20];
} plant;

/*******************************************************************//*
@param d - plant attributes from data
@param q - attributes from query
@return - probably return a plant 
*//*******************************************************************/
float dist(float d[], float q[]){
  return fabs(d[0]-q[0]) + fabs(d[1]-q[1]) + fabs(d[2]-q[2]) + fabs(d[3]-q[3]);
}

int main(int argc, char ** argv){
  /*******************************************************************/
  /* 1. Initialization */
  /*******************************************************************/
  int myproc, nprocs, len;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myproc);
  MPI_Request request[100];
  MPI_Status status;

  char *buf = NULL; 
  size_t leng; float a, b, c, d; char ch; char e[20];
  plant data[100];
  int plant_idx=0;

  /*******************************************************************/
  /* 2. read data input */
  /*******************************************************************/
  if(myproc==0){
    FILE *fp = fopen(argv[1],"r");
    while(getline(&buf, &leng, fp)>0){
      sscanf(buf, "%f %[,] %f %[,] %f %[,] %f %[,] %s", &a, &ch, &b, &ch, &c, &ch, &d, &ch, e);
      //printf("%f %f %f %f %s\n", a, b, c, d, e);
      data[plant_idx].attr[0]=a;
      data[plant_idx].attr[1]=b;
      data[plant_idx].attr[2]=c;
      data[plant_idx].attr[3]=d;
      strcpy(data[plant_idx].class,e);
      plant_idx++;
    }
    fclose(fp);
  }
  MPI_Barrier(MPI_COMM_WORLD);

  /*******************************************************************/
  /* 4. send plant attributes to all procs with rank greater than 0 */ 
  /*******************************************************************/
  float attr[4];
  if(myproc==0){ 
    for(int i=1;i<nprocs;i++){ MPI_Isend(&data[i-1].attr, 4, MPI_FLOAT, i, 0, MPI_COMM_WORLD, &request[i]); }
  } else { MPI_Irecv(&attr, 4, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &request[0]); }
  MPI_Barrier(MPI_COMM_WORLD);

  /*******************************************************************/
  /* 5. send plant class to all procs with rank greater than 0 */ 
  /*******************************************************************/
  char class[20];
  if(myproc==0){ 
    for(int i=1;i<nprocs;i++){ MPI_Isend(&data[i-1].class, 20, MPI_CHAR, i, 0, MPI_COMM_WORLD, &request[i]); }
  } else { MPI_Irecv(&class, 20, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &request[0]); }
  MPI_Barrier(MPI_COMM_WORLD);

  /*******************************************************************//*
   6. read queries from file and iterate through them.
  *//*******************************************************************/
  plant_idx=0;
  plant query[100]; 
  if(myproc==0){
    FILE *fp2 = fopen(argv[2],"r");
    while(getline(&buf, &leng, fp2)>1){
      sscanf(buf, "%f %[,] %f %[,] %f %[,] %f", &a, &ch, &b, &ch, &c, &ch, &d);
      //printf("%f %f %f %f\n", a, b, c, d);
      query[plant_idx].attr[0]=a; query[plant_idx].attr[1]=b;
      query[plant_idx].attr[2]=c; query[plant_idx].attr[3]=d;
      plant_idx++;
    }
    fclose(fp2);
  }
  MPI_Barrier(MPI_COMM_WORLD);

  /*******************************************************************/
  /* 7. send data_size to all procs with rank greater than 0 */
  /*******************************************************************/
  int query_size;
  if(myproc==0){
    for(int i=1;i<nprocs;i++){ MPI_Isend(&plant_idx, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &request[i]); }
    query_size=plant_idx; // unique for query_size 
  } else { MPI_Irecv(&query_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request[0]); }
  MPI_Barrier(MPI_COMM_WORLD);

  /*******************************************************************/
  /* 8. send query attr to all procs with rank greater than 0 */
  /*******************************************************************/
  float qattr[4];
  for(int j=0;j<query_size;j++){
    if(myproc==0){
      for(int i=1;i<nprocs;i++){
        //printf("\t\tsend to: %d 2coll[i]: %f %f %f %f\n", i, query[j].attr[0], query[j].attr[1], query[j].attr[2], query[j].attr[3]);
        MPI_Isend(&query[j].attr, 4, MPI_FLOAT, i, 0, MPI_COMM_WORLD, &request[i]);
        //MPI_Wait(&request[i], &status);
      }
    } else {
      MPI_Irecv(&qattr, 4, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &request[nprocs+myproc]);
      //MPI_Recv(&qattr, 4, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &status);
      MPI_Wait(&request[nprocs+myproc], &status);
      printf("\t\tproc: %d qattr: %f %f %f %f\n", myproc, qattr[0], qattr[1], qattr[2], qattr[3] );
    }
    MPI_Barrier(MPI_COMM_WORLD);
  }

  float manhattan=-1;
  if(myproc>0) {
    manhattan=dist(attr, qattr);
    printf("proc: %d manhattan: %f\n", myproc, manhattan);
  }
  MPI_Finalize();
}















/*******************************************************************//*
Author: Gabriel Hofer
Date: December 7, 2020
Course: CSC-410
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

/* comparator for qsort */
int cmp(const void * a, const void * b){
  return ((float *)a)[0] > ((float *)b)[0];
}

/*******************************************************************//*
  @param d - plant attributes from data
  @param q - attributes from query
  @return  - return the manhattan distance between the two vectors
*//*******************************************************************/
float dist(float d[], float q[]){
  return fabs(d[0]-q[0]) + fabs(d[1]-q[1]) + fabs(d[2]-q[2]) + fabs(d[3]-q[3]);
}

/*******************************************************************//*
  Main's Local Variables:
-----------------------------------------------------------------------
  data_size  - number of lines/records in the data file
  query_size - number of lines/records in the query file
  plant_idx  - an iterator variable for traversing an array of plants

  fp   - file pointer to the file we're reading from 
  buf  - read a whole line/record from a file
  leng - integer representing the length of the string read in getline

  a - the first field in a record 
  b - the second field in a record
  c - the third field in a record
  d - the fourth field in a record
  e - string for the class of a plant 

  data  - array containing all of the records from the data file
  attr  - used for sending/receiving plant attributes (from the data file)
				  to other processes.
  class - used for sending/receiving plant classes to other processes
					from the data file.
  qattr - used for sending/receiving plant attributes (from the query file
					to other processes.
  man_proc - pair of floats which maps manhattan distances to the index
						 of the record that corresponds to that manhattan distance.
  arr   - array of pairs of floats containing all of the man_proc pairs.

*//*******************************************************************/
int main(int argc, char ** argv){
  /*******************************************************************/
  /* 1. Initialization */
  /*******************************************************************/
  int myproc, nprocs, len;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myproc);
  MPI_Request request[1000];
  MPI_Status status;

  char *buf = NULL; 
  size_t leng; float a, b, c, d; char ch; char e[20];
  plant data[1000];
  int plant_idx=0;
  int data_size;
  int query_size;

  /*******************************************************************/
  /* 2. read data input */
  /*******************************************************************/
  if(myproc==0){
    FILE *fp = fopen(argv[1],"r");
    while(getline(&buf, &leng, fp)>0){
      sscanf(buf, "%f %[,] %f %[,] %f %[,] %f %[,] %s", &a, &ch, &b, &ch, &c, &ch, &d, &ch, e);
      data[plant_idx].attr[0]=a;
      data[plant_idx].attr[1]=b;
      data[plant_idx].attr[2]=c;
      data[plant_idx].attr[3]=d;
      strcpy(data[plant_idx].class,e);
      plant_idx++;
    }
    data_size=plant_idx;
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

  /*******************************************************************//*
    5. send plant class to all procs with rank greater than 0 
  *//*******************************************************************/
  char class[20];
  if(myproc==0){ 
    for(int i=1;i<nprocs;i++){ MPI_Isend(&data[i-1].class, 20, MPI_CHAR, i, 0, MPI_COMM_WORLD, &request[i]); }
  } else { MPI_Irecv(&class, 20, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &request[0]); }
  MPI_Barrier(MPI_COMM_WORLD);

  /*******************************************************************//*
    6. read queries from file and iterate through them.
  *//*******************************************************************/
  plant_idx=0;
  plant query[1000]; 
  if(myproc==0){
    FILE *fp2 = fopen(argv[2],"r");
    while(getline(&buf, &leng, fp2)>1){
      sscanf(buf, "%f %[,] %f %[,] %f %[,] %f", &a, &ch, &b, &ch, &c, &ch, &d);
      query[plant_idx].attr[0]=a; 
      query[plant_idx].attr[1]=b;
      query[plant_idx].attr[2]=c; 
      query[plant_idx].attr[3]=d;
      plant_idx++;
    }
    query_size=plant_idx; // unique for query_size 
    fclose(fp2);
  }
  MPI_Barrier(MPI_COMM_WORLD);

  /*******************************************************************/
  /* 7. send data_size to all procs with rank greater than 0 */
  /*******************************************************************/
  if(myproc==0){
    for(int i=1;i<nprocs;i++){ MPI_Isend(&plant_idx, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &request[i]); }
  } else { MPI_Irecv(&query_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request[0]); }
  MPI_Barrier(MPI_COMM_WORLD);

  /*******************************************************************/
  /* 8. send query attrributes to all procs with rank greater than 0 */
  /*******************************************************************/
  float qattr[4];
  for(int j=0;j<query_size;j++){
    if(myproc==0){
      for(int i=1;i<nprocs;i++){ MPI_Isend(&query[j].attr, 4, MPI_FLOAT, i, 0, MPI_COMM_WORLD, &request[i]); }
    } else { MPI_Irecv(&qattr, 4, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &request[nprocs+myproc]); }
    MPI_Barrier(MPI_COMM_WORLD);

    /*******************************************************************//*
      For the current query, calculate manhattan distance between all vectors
    *//*******************************************************************/
    float manhattan=-1;
    if(myproc>0) { manhattan=dist(attr, qattr); }
    MPI_Barrier(MPI_COMM_WORLD);

    /*******************************************************************//*
      Send manhattan distance back to the root process 
    *//*******************************************************************/
    float man_proc[2];
    man_proc[0]=manhattan;
    man_proc[1]=(float)myproc;

    float arr[1000][2];
    if(myproc==0){ 
      for(int i=1;i<nprocs;i++){ MPI_Irecv(&arr[i-1], 2, MPI_FLOAT, i, 0, MPI_COMM_WORLD, &request[i]); }
    } else { MPI_Isend(&man_proc, 2, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &request[0]); }
    MPI_Barrier(MPI_COMM_WORLD);
    /*******************************************************************//*
     Sort all of the distances, print the first k.
    *//*******************************************************************/
    if(myproc==0){
      qsort(arr, data_size, 2*sizeof(float), cmp);
      int K=atoi(argv[3]);
      float rattr[4]={0,0,0,0};

      if(0==strcmp(argv[4],"r")){
        /* regression */
        /* iterate through the closest K vectors and calculate the arith. mean of attributes */
        for(int i=0;i<K;i++){
          rattr[0]+=data[(int)arr[i][1]-1].attr[0]/(float)K;
          rattr[1]+=data[(int)arr[i][1]-1].attr[1]/(float)K;
          rattr[2]+=data[(int)arr[i][1]-1].attr[2]/(float)K;
          rattr[3]+=data[(int)arr[i][1]-1].attr[3]/(float)K;
        }
				/* print the mean of the closest k vectors */
        printf("MEAN: %f %f %f %f\n", rattr[0], rattr[1], rattr[2], rattr[3]);
      } else if(0==strcmp(argv[4],"m")){
        /* iterate through the closest K vectors and count the frequency of each class of plant */
        int Iris_virginica=0;
        int Iris_versicolor=0;
        int Iris_setosa=0;
        for(int i=0;i<K;i++){
          if(strcmp(data[(int)arr[i][1]].class,"Iris-virginica")==0)
            Iris_virginica+=1;
          if(strcmp(data[(int)arr[i][1]].class,"Iris-versicolor")==0)
            Iris_versicolor+=1;
          if(strcmp(data[(int)arr[i][1]].class,"Iris-setosa")==0)
            Iris_setosa+=1;
        }
        /* print the counts for the three plant classes */
        printf("iris-virginica:  %d\n", Iris_virginica);
        printf("iris-versicolor: %d\n", Iris_versicolor);
        printf("iris-setosa:     %d\n", Iris_setosa);
        /* find the mode by finding the class with the max cnt */
        int mode_idx=-1;
        if(Iris_virginica>=Iris_versicolor && Iris_virginica>=Iris_setosa)
          mode_idx=0;
        else if(Iris_versicolor>=Iris_virginica && Iris_versicolor>=Iris_setosa)
          mode_idx=1;
        else if(Iris_setosa>=Iris_virginica && Iris_setosa>=Iris_versicolor)
          mode_idx=2;
        /* print the mode of the closest k vectors */
        if(mode_idx==0) printf("MODE: Iris-virginica\n");
        if(mode_idx==1) printf("MODE: Iris-versicolor\n");
        if(mode_idx==2) printf("MODE: Iris-setosa\n");
      }
    }
    MPI_Barrier(MPI_COMM_WORLD);
  }
  MPI_Finalize();
}









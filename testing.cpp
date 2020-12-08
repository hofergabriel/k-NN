#include <bits/stdc++.h>
using namespace std;

typedef struct {
  float attr[4]; // sepal_l, sepal_w, petal_l, petal_w;
  char class_[20];
} plant;

/* comparator for qsort */
int cmp(const void * a, const void * b){
  return ((float *)a)[0] > ((float *)b)[0];
}

float dist(float d[], float q[]){
  return fabs(d[0]-q[0]) + fabs(d[1]-q[1]) + fabs(d[2]-q[2]) + fabs(d[3]-q[3]);
}


int main(int argc, char ** argv){

  char *buf = NULL; 
  size_t leng; float a, b, c, d; char ch; char e[20];
  plant data[1000];
  int plant_idx=0;
  int data_size;
  int query_size;
  int K=atoi(argv[3]);

  /*******************************************************************/
  /* read data input */
  /*******************************************************************/
  FILE *fp = fopen(argv[1],"r");
  while(getline(&buf, &leng, fp)>0){
    sscanf(buf, "%f %[,] %f %[,] %f %[,] %f %[,] %s", &a, &ch, &b, &ch, &c, &ch, &d, &ch, e);
		printf("%f %f %f %f %s\n", a, b, c, d, e);
    data[plant_idx].attr[0]=a;
    data[plant_idx].attr[1]=b;
    data[plant_idx].attr[2]=c;
    data[plant_idx].attr[3]=d;
    strcpy(data[plant_idx].class_,e);
    plant_idx++;
  }
  data_size=plant_idx;
  fclose(fp);

  /*******************************************************************/
  /* read query input */
  /*******************************************************************/
  plant_idx=0;
  plant query[1000]; 
  FILE *fp2 = fopen(argv[2],"r");
  while(getline(&buf, &leng, fp2)>1){
    sscanf(buf, "%f %[,] %f %[,] %f %[,] %f", &a, &ch, &b, &ch, &c, &ch, &d);
		printf("%f %f %f %f\n", a, b, c, d);
    query[plant_idx].attr[0]=a; 
    query[plant_idx].attr[1]=b;
    query[plant_idx].attr[2]=c; 
    query[plant_idx].attr[3]=d;
    plant_idx++;
  }
  query_size=plant_idx; // unique for query_size 
  fclose(fp2);

  float arr[1000][2];
  for(int j=0;j<query_size;j++){

    /*******************************************************************//*
      For the current query, calculate manhattan distance between all vectors
    *//*******************************************************************/
    for(int i=0;i<data_size;i++){
      arr[i][0]=dist(data[i].attr, query[j].attr);
      arr[i][1]=i;
    }

    qsort(arr, data_size, 2*sizeof(float), cmp);
		for(int i=0;i<data_size;i++){
			printf("%f %f\n", arr[i][0], arr[i][1]);	
		}

    float rattr[4]={0,0,0,0};

    if(0==strcmp(argv[4],"r")){
      /* regression */
      /* iterate through the closest K vectors and calculate the arith. mean of attributes */
      for(int i=0;i<K;i++){
        rattr[0]+=data[(int)arr[i][1]].attr[0]/(float)K;
        rattr[1]+=data[(int)arr[i][1]].attr[1]/(float)K;
        rattr[2]+=data[(int)arr[i][1]].attr[2]/(float)K;
        rattr[3]+=data[(int)arr[i][1]].attr[3]/(float)K;
      }
			/* print the mean of the closest k vectors */
      printf("MEAN: %f %f %f %f\n", rattr[0], rattr[1], rattr[2], rattr[3]);

    } else if(0==strcmp(argv[4],"m")){
      /* iterate through the closest K vectors and count the frequency of each class of plant */
      int Iris_virginica=0;
      int Iris_versicolor=0;
      int Iris_setosa=0;
      for(int i=0;i<K;i++){
        if(strcmp(data[(int)arr[i][1]].class_,"Iris-virginica")==0)
          Iris_virginica+=1;
        if(strcmp(data[(int)arr[i][1]].class_,"Iris-versicolor")==0)
          Iris_versicolor+=1;
        if(strcmp(data[(int)arr[i][1]].class_,"Iris-setosa")==0)
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

  return 0;
}


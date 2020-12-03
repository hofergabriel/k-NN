#include <stdio.h>
#include <stdlib.h>
#include <math.h>	// To use the sqrt function
#include <string.h>	// To use the memcpy function
// #include <time.h>	// To seed the randomizer

//#include <mpi.h>	// Main parallel library
#include <unistd.h>	// Needed for gethostname and sleep

struct plant {
  int sepal_l, sepal_w, petal_l, petal_w;
	char class[20];
};

//void kNN(){ }
/*
float dist(plant a, plant b){
	return (float)1.0;
}*/

int main(int argc, char ** argv){
	char *buf = NULL;
	size_t len;
	float a, b, c, d;
	char ch;
	FILE *fp = fopen(argv[1],"r");
	while(getline(&buf, &len, fp)>0){
		getline(&buf, &len, fp);
		sscanf(buf, "%f %[,] %f %[,] %f %[,] %f", &a, &ch, &b, &ch, &c, &ch, &d);
		printf("%f %f %f %f\n", a, b, c, d);
	}



}










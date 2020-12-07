default: 
	mpicc k-NN.c -o k-NN -lm 
	mpirun -np 6 ./k-NN data queries

small:
	mpicc k-NN.c -o k-NN -lm 
	mpirun -np 6 ./k-NN small.data queries

iris: 
	mpicc k-NN.c -o k-NN -lm 
	mpirun -np 151 ./k-NN iris.data queries

clean:
	rm k-NN

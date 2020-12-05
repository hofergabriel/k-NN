default: 
	mpicc k-NN.c -o k-NN -lm 
	mpirun -np 6 ./k-NN data queries

clean:
	rm k-NN

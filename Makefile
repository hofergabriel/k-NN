default: 
	mpicc k-NN.c -o k-NN -lm 
	mpirun -np 2 ./k-NN data

clean:
	rm k-NN

default: 
	mpicc k-NN.c -o k-NN -lm
	mpirun -np 2 ./k-NN

input:
	mpicc k-NN.c -o k-NN -lm < in
	mpirun -np 2 ./k-NN < in



default: 

small_reg:
	mpicc k-NN.c -o k-NN -lm 
	mpirun -np 6 ./k-NN small.data queries 3 r

small_mode:
	mpicc k-NN.c -o k-NN -lm 
	mpirun -np 6 ./k-NN small.data queries 3 m

iris: 
	mpicc k-NN.c -o k-NN -lm 
	mpirun -np 151 ./k-NN iris.data queries 3 r

clean:
	rm k-NN

doc: 
	pdflatex doc.tex
	evince doc.pdf



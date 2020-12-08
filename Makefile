default: 

small_reg:
	mpicc k-NN.c -o k-NN -lm 
	mpirun -np 6 ./k-NN small.data queries.data 3 r

small_mode:
	mpicc k-NN.c -o k-NN -lm 
	mpirun -np 6 ./k-NN small.data queries.data 3 m

iris_reg: 
	mpicc k-NN.c -o k-NN -lm 
	mpirun -np 151 ./k-NN iris.data queries.data 3 r

iris_mode:
	mpicc k-NN.c -o k-NN -lm 
	mpirun -np 151 ./k-NN iris.data queries.data 3 m

testing_iris_m:
	g++ testing.cpp
	./a.out iris.data queries.data 3 m

testing_iris_r:
	g++ testing.cpp
	./a.out iris.data queries.data 3 r

testing_small_m:
	g++ testing.cpp
	./a.out small.data queries.data 3 m

testing_small_r:
	g++ testing.cpp
	./a.out small.data queries.data 3 r

doc: 
	pdflatex doc.tex
	evince doc.pdf

clean:
	rm k-NN


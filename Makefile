default:
	g++ -fopenmp -g -o serialSolver serialSolver.cpp 

clean:
	/bin/rm -rf *~ *.o serialSolver
	
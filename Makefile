default:
	g++ -fopenmp -g -o serialSolver serialSolver.cpp
	g++ -fopenmp -g -o serialSolver2 serialSolver2.cpp 

clean:
	/bin/rm -rf *~ *.o serialSolver serialSolver2
	
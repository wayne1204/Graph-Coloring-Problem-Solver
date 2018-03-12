ColorTest: clean File.o Proof.o Solver.o graph.o graphTest.o
	g++ -o $@ -g File.o Proof.o Solver.o graph.o  graphTest.o

File.o: File.cpp
	g++ -c -g File.cpp

Proof.o: Proof.cpp
	g++ -c -g Proof.cpp

Solve.o: Solver.cpp
	g++ -c -g Solver.cpp

graph.o: graph.cpp
	g++ -c -g graph.cpp

graphTest.o: graphTest.cpp
	g++ -c -g graphTest.cpp

clean:
	rm -f *.o graphTest tags

/****************************************************************************
  FileName     [ graphTest.cpp ]
  PackageName  [ sat ]
  Synopsis     [ main file of graph-coloring problem ]
  Author       [ Ping-Wei Hunag ]
	106-1  EDA project
****************************************************************************/
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include "sat.h"
#include "graph.h"
using namespace std;

int myStrNCmp(const string& s1, const string& s2, unsigned n);

int main(int argc, char**argv){
 //  // initialize graph
	if(argc < 4 || argc > 5){
		cerr<<"[ERROR] Usage: ./satTest <#node> <#color> <conductivity> [-cnf]"<<endl;
		return -1;
	}
	Graph g = Graph (atoi(argv[1]), atoi(argv[2]));
	double c = atof(argv[3]);
	if( c <= 0 || c >1){
		cerr<<"[ERROR] Illegal conductivity!! conductivity must within 0 and 1\n";
		return -1;
	}
	g.setConduct(c);
    g.initNode();
    g.initLink();
	if(argc > 4){
		if(myStrNCmp("-Cnf", argv[4], 2) == 0) {
			g.genCNF();
			stringstream ss;
			ss<<argv[1]<<"_"<<argv[2]<<"_CNF";
			g.writeCNF(ss.str());
			g.reportCNF();
			return 0;
		}
		cerr<<"[ERROR] Unknown usage \""<<argv[3]<<"\""<<endl;
	}
	else{
		//sat solver
		clock_t tStart = clock();
		SatSolver solver;
		solver.initialize();
		g.genVariable(solver);
		g.genProofModel(solver);
		bool result;
		result = solver.solve();
		g.reportResult(solver, result);
		printf("Time taken: %.3fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
		return 0;
	}
}

int
myStrNCmp(const string& s1, const string& s2, unsigned n)
{
   assert(n > 0);
   unsigned n2 = s2.size();
   if (n2 == 0) return -1;
   unsigned n1 = s1.size();
   assert(n1 >= n);
   for (unsigned i = 0; i < n1; ++i) {
      if (i == n2)
         return (i < n)? 1 : 0;
      char ch1 = (isupper(s1[i]))? tolower(s1[i]) : s1[i];
      char ch2 = (isupper(s2[i]))? tolower(s2[i]) : s2[i];
      if (ch1 != ch2)
         return (ch1 - ch2);
   }
   return (n1 - n2);
}

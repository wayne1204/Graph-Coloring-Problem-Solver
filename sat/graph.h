/****************************************************************************
  FileName     [ graph.h]
  PackageName  [ sat ]
  Synopsis     [ define graph and nodes and CNF constraints]
  Author       [ Ping-Wei Hunag ]
    106-1  EDA project
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <vector>
#include "sat.h"
using namespace std;

class Node
{
public:
   Node(unsigned i): _gid(i)  {	 
   }
   ~Node() {
   		delete[] _logvars;
   }
   void initvar(int& LogColor) {_logvars = new Var [LogColor]; }
   int getID()   const{  return _gid;  }
   
   Var* returnArr()const{ return _logvars; }
   Var getLogVar(const int& index) const { return  _logvars[index]; }
   void setLogVar(const Var v,const int index) { _logvars[index] = v; }

   int getLinkNum()const{ return _linkList.size(); }
   void  addLink(Node* _n){  _linkList.push_back(_n);  }
   Node* getLinkVar(int i)const {  return _linkList[i]; }
   void printLink()const;	

private:
   Var*           _logvars;     
   vector<Node*>  _linkList;  //save array index not gid
   unsigned       _gid;       // for debugging purpose... 
};

class Graph
{
public:
	Graph(int num, int c) :NUMBER(num), COLOR(c){
		if (COLOR==1) LogColor=1;
		else LogColor = log2(COLOR-1)+1;
		_variable = COLOR * NUMBER;
	}
	~Graph(){}

	//initiallize function
	void initNode();
	void initLink(); 
	void setConduct(double c){ _conduct = c;}
	// for SAT solver
	void genVariable(SatSolver& s);
	void genProofModel(SatSolver& s);
	void assignCNF(SatSolver& s, Node* node, int level);
	bool isLink(size_t n1, size_t n2);
	bool checklink(Node* node);
	bool isDisconnect(Node* node);
	void printEdge();
	void reportResult(const SatSolver& solver, bool result);
	// for outputing CNF file
	void genCNF();
	void indeCNF(Node* node, int level);
	void pre_assignCNF(Node* node, int level);
	void NodeCNF(int& num,int& exclude);
	void AdjCNF(int num1,int num2,int& exclude);
	void XorCNF(int num1,int num2);
	void writeCNF(const string& filename) const;
	void reportCNF()const;
	int getLinksNum() const;
	int getLitnum() const;
	double getDensity()const;
private:
	vector<vector<int> > _clauses;
	vector<Node*> _nodes;
	vector<Node*> storing;
	vector<Node*> storing2;
	double _conduct;
	int _variable;
	int NUMBER ;
	int COLOR ;
	int LogColor;
};

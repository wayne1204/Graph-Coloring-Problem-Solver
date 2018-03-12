/****************************************************************************
  FileName     [ graph.cpp ]
  PackageName  [ sat ]
  Synopsis     [ define class graph and nodes and CNF constraints]
  Author       [ Ping-Wei Hunag ]
    106-1  EDA project
****************************************************************************/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include <math.h>
#include "sat.h"
#include "graph.h"
using std::rand;
using std::srand;
using namespace std;
// for printing edge list and result color
#define PRINT_RESULT
#define POWER_CONSTRAINT

void Node::printLink() const{
    cout<<"Node"<<setw(3)<<_gid<<" connect to";
    for(size_t i=0; i<getLinkNum(); ++i)
        cout<<setw(3)<<_linkList[i]->getID();
}

void Graph::initNode(){
	for(int i=0; i<NUMBER; ++i){
		_nodes.push_back(new Node(i));
		_nodes[i]->initvar(LogColor);
	}
}

void Graph::initLink(){
  srand (1);
  for(int i=0; i<NUMBER-1; ++i){
    for(int j=i+1; j<NUMBER; ++j){
      double num=(double) rand() / (RAND_MAX );
      if(num <= _conduct){
        _nodes[i]->addLink(_nodes[j]);
        _nodes[j]->addLink(_nodes[i]);
      }
    }
  }
}

void Graph::genVariable(SatSolver& s){
	for (size_t i = 0; i < NUMBER; ++i) {            //variable init
	    for(int j = 0; j < LogColor; ++j) {
	       Var v = s.newVar();
	       _nodes[i]->setLogVar(v,j);
	    }
	}
}
void Graph::genProofModel(SatSolver& s){
    int diff_num = pow(2, LogColor) - COLOR;
    if(diff_num != 0){
        for(int i=0; i<NUMBER; ++i)
            s.addNodeCNF(_nodes[i]->returnArr(), diff_num, LogColor);
    }
    for(int i=0; i<NUMBER-1; ++i){
        for(int j=0, k = _nodes[i]->getLinkNum(); j<k; ++j){
             if(i < _nodes[i]->getLinkVar(j)->getID())
                 s.addAdjCNF(_nodes[i]->returnArr(), _nodes[i]->getLinkVar(j)->returnArr(), LogColor, diff_num);
        /*Var* _xor = new Var[LogColor];
        for(int n=0;n<LogColor; ++n){
          _xor[n] = s.newVar();
          s.AdjXorCNF(_xor[n],_nodes[i]->getLogVar(n),_nodes[i]->getLinkVar(j)->getLogVar(n));
        }
        s.addArrCNF(_xor, LogColor);*/
        }
    }	
    #ifdef POWER_CONSTRAINT
    int max_link = 0;
    Node* max_node;
    for(size_t i =0, k= _nodes.size(); i<k; i++){
        if(_nodes[i]->getLinkNum() > max_link){
            max_link = _nodes[i]->getLinkNum();
            max_node = _nodes[i];
        }
    }
    assignCNF(s,max_node,  LogColor-1);
    #endif
}
 
void Graph::assignCNF(SatSolver& s, Node* node, int level){
    storing.push_back(node);
    // cout<<"pre-assign node: "<<node->getID()<<endl;
    for(int i = 0; i < LogColor; i++){
        if((LogColor-i) <= level +1){  // reverse 
            s.addlitCNF(node->getLogVar(i),true);  
        }
        else { 
            s.addlitCNF(node->getLogVar(i),false); 
        }
    }
    if(level <= 0) return;
    
    int max_link = 0;
    Node* max_node = NULL;
    for(size_t i =0, k= node->getLinkNum(); i<k; i++){
        if( checklink(node->getLinkVar(i)) && node->getLinkVar(i)->getLinkNum() > max_link){
            max_link = node->getLinkVar(i)->getLinkNum();
            max_node = node->getLinkVar(i);
        }
    }
    if(max_node != NULL)
    assignCNF(s, max_node, level-1);
}
bool Graph::isLink(size_t n1, size_t n2){
    bool _isLink = false;
    if(n1 == n2) return false;
    for(size_t i =0, k = _nodes[n1]->getLinkNum(); i<k; ++i){
        if(_nodes[n1]->getLinkVar(i)->getID() == n2){
            _isLink = true;
            break;
        }
    }
    return _isLink;
}

// check whether node is connected to the nodes in the storing
bool Graph::checklink(Node* node){
    for(size_t i=0, k=storing.size(); i<k; ++i){
        bool check = false;
        if(storing[i]->getID() == node->getID())    {
            return false;
        }
        for(size_t pos =0; pos< node->getLinkNum(); ++pos){
            if(storing[i]->getID() == node->getLinkVar(pos)->getID())
                check =true;
        }
        if(!check) return false;
    }
    return true;
}
bool Graph::isDisconnect(Node* node){
    for(size_t i=0, k=storing2.size(); i<k; ++i){
        bool check = false;
        if(storing2[i]->getID() == node->getID()) {
            return false;
            cout<<storing2[i]->getID()<<endl;
        }
        if( !isLink(node->getID(),storing2[i]->getID()) ){
            check =true;
        }  
        if(!check ) return false;
 
    }
    return true;
}

void Graph::printEdge(){	
	cout<<"=============================[Edge List]================================"<<endl;
	for(int i=0;i<NUMBER;i++){
		_nodes[i]->printLink();
		cout<<endl;
	}

}
void Graph::reportResult(const SatSolver& solver, bool result){
 	#ifdef PRINT_RESULT
 	printEdge();
	#endif

    solver.printStats();
    cout<<"#nodes :"<<NUMBER<<endl;
    cout<<"#color :"<<COLOR<<endl;
    cout <<"result :"<<(result? "SAT" : "UNSAT") << endl;
    #ifdef PRINT_RESULT
    if (result) {
        for (size_t i = 0; i <  NUMBER; ++i){
            int color_num = COLOR - pow(2,LogColor);
        	cout <<"Node"<<setw(3)<<i<<": color #";
            for(int j=0; j<LogColor; ++j)
                color_num += solver.getValue(_nodes[i]->getLogVar(j)) * pow(2,j);
        	// for(int j=LogColor-1; j>=0; --j){
        	// 	cout<<setw(2) << solver.getValue(_nodes[i]->getLogVar(j)) ;
        	// }
	    	cout<<color_num<<endl;
        }
    }
    #endif
}
void Graph::genCNF(){
    int diff_num = pow(2, LogColor) - COLOR;


    if(diff_num != 0){
        for(int i=0; i<NUMBER; ++i)
        NodeCNF(i,diff_num);
    }
    for(int i=0; i<NUMBER-1; ++i){
        for(int j=0;j< _nodes[i]->getLinkNum(); ++j){
            if(i < _nodes[i]->getLinkVar(j)->getID())
             //   XorCNF(i, _nodes[i]->getLinkVar(j)->getID());
            AdjCNF(i, _nodes[i]->getLinkVar(j)->getID(), diff_num);
        }
    }  
    #ifdef POWER_CONSTRAINT
    int max_link = 0,min_link = NUMBER;
    Node* max_node; 
    for(size_t i =0, k= _nodes.size(); i<k; i++){
        if(_nodes[i]->getLinkNum() > max_link){
            max_link = _nodes[i]->getLinkNum();
            max_node = _nodes[i];
        }
    }
    
    cout<<"pre-assigning maximal clique..." <<endl; 
    pre_assignCNF(max_node,COLOR);  
    cout<<endl;
    cout<<"pre-assigning maximal indenpendent set..." <<endl; 
    indeCNF(max_node, COLOR);
    cout<<endl;    
    #endif
}
void Graph::indeCNF(Node* node, int level){
    vector<int> _lits;
    int lit, divider;
    int color_num = pow(2, LogColor) - COLOR;
    storing2.push_back(node);
    cout<<" "<<node->getID();
    
    for(int i = 1; i <= LogColor; i++){
        divider = pow(2, i-1);
        lit = (color_num/divider ) % 2 ? (node->getID()*LogColor+i) : (node->getID()*LogColor+i)*(-1);
        //if  divisible, then literal inverted   indivisible : divisible
        _lits.push_back(lit);
        _clauses.push_back(_lits);
        _lits.clear();
    }
    if(level <= 0) return;
    
    int max_link = 0;
    Node* max_node = 0;

    for(size_t i =0; i<NUMBER; i++){
        if( isDisconnect(_nodes[i]) && _nodes[i]->getLinkNum() > max_link){
            max_link = _nodes[i]->getLinkNum();
            max_node = _nodes[i];
        }
    }

    if(max_node != NULL)
    indeCNF(max_node, level-1);
}
void Graph::pre_assignCNF(Node* node, int level){
    vector<int> _lits;
    int lit, divider;
    int color_num = pow(2, LogColor) - COLOR + (COLOR - level);  // diff_num + #pre_assign node
    storing.push_back(node);
    cout<<" "<<node->getID();
    for(int i = 1; i <= LogColor; i++){
        divider = pow(2, i-1);
        lit = (color_num/divider ) % 2 ? (node->getID()*LogColor+i) : (node->getID()*LogColor+i)*(-1);
        //if  divisible, then literal inverted   indivisible : divisible
        _lits.push_back(lit);
        _clauses.push_back(_lits);
        _lits.clear();
    }
    if(level <= 0) return;
    
    int max_link = 0;
    Node* max_node = 0;
    for(size_t i =0, k= node->getLinkNum(); i<k; i++){
        if( checklink(node->getLinkVar(i)) && node->getLinkVar(i)->getLinkNum() > max_link){
            max_link = node->getLinkVar(i)->getLinkNum();
            max_node = node->getLinkVar(i);
        }
    }
    if(max_node != NULL)
        pre_assignCNF(max_node, level-1);
}

void Graph::NodeCNF(int& num,int& exclude){
	vector<int> _lits;
	int lit,divider;
    for(int i=0;i<exclude;i++){
        for(int j=1; j <= LogColor; j++){
            divider = pow(2,j-1);
            lit = (i/divider ) % 2 ? (num*LogColor+j)*(-1) : (num*LogColor+j);
            //if not divisible, then literal inverted
            _lits.push_back(lit);
        }
        _clauses.push_back(_lits);
        _lits.clear();
    }
}

void Graph::AdjCNF(int num1,int num2,int& exclude){
 	vector<int> _lits;
 	int lit1,lit2;
 	for(int i = exclude; i< pow(2,LogColor); i++){
    	for(int j=1; j <= LogColor; j++){
            int divider = pow(2,j-1);
            lit1 = (i/divider ) % 2 ? (num1*LogColor+j)*(-1) : (num1*LogColor+j);
            lit2 = (i/divider ) % 2 ? (num2*LogColor+j)*(-1) : (num2*LogColor+j);
            //if not divisiblet, then literal inverted
      	    _lits.push_back(lit1);         
    	    _lits.push_back(lit2);
        }    
    _clauses.push_back(_lits);
    _lits.clear();       
    }
}
void Graph::XorCNF(int num1,int num2){
    vector<int> _lits;
    for(int i = 1; i <= LogColor; ++i){
        int lit1,lit2,lit_f;
        _variable++;
        lit1 = num1*LogColor+i;     
        lit2 = num2*LogColor+i;     
        // a + b + f'
        _lits.push_back(lit1);    
        _lits.push_back(lit2);
        _lits.push_back(_variable*(-1));
        _clauses.push_back(_lits);
        _lits.clear(); 
        // a'+ b'+ f'
        _lits.push_back(lit1*(-1));         
        _lits.push_back(lit2*(-1));
        _lits.push_back(_variable*(-1));
        _clauses.push_back(_lits);
        _lits.clear(); 
        // a + b'+ f
        _lits.push_back(lit1);         
        _lits.push_back(lit2*(-1));
        _lits.push_back(_variable);
        _clauses.push_back(_lits);
        _lits.clear(); 
        // a'+ b + f
        _lits.push_back(lit1*(-1));         
        _lits.push_back(lit2);
        _lits.push_back(_variable);
        _clauses.push_back(_lits);
        _lits.clear(); 
    }
    for(int j=0; j<LogColor;j++)
        _lits.push_back(_variable-j);
    _clauses.push_back(_lits);
}
int Graph::getLitnum() const{
	int total_lit = 0;
	for(size_t i=0;i<_clauses.size();++i){
		total_lit += _clauses[i].size();
	}
	return total_lit;
}

int Graph::getLinksNum() const{
    int total_link = 0;
    for(size_t i = 0; i<_nodes.size(); ++i){
        total_link += _nodes[i]->getLinkNum();
    }
    return total_link/2;
}
double Graph::getDensity()const{
    double total_link = NUMBER*NUMBER/2;
    return (getLinksNum()/total_link) * 100;
}
void Graph::writeCNF(const string& filename) const
{
   // Open output file
   ofstream outfile(filename.c_str(), ios::out);

   outfile << "p cnf " << _variable << " " <<_clauses.size()<<endl;

   // Clauses
    for(size_t i=0, n = _clauses.size(); i < n; ++i) {
        for(size_t j = 0, k=_clauses[i].size(); j<k; ++j)
            outfile << _clauses[i][j]<< " " ;
        outfile << "0\n";
    }
   outfile.close();
}

void Graph::reportCNF()const{
    cout<<"Successfully output CNF file!"<<endl;
    cout<<"=======[graph info]========\n";
    cout<<" #nodes  : "<<NUMBER<<endl;
    cout<<" #color  : "<<COLOR<<endl;
    cout<<" #links  : "<<getLinksNum()<<endl;
    cout<<" density : "<<setprecision(3)<<getDensity()<<" percent \n";
    cout<<"========[CNF info]=========\n";
    cout<<" #Variables: "<<_variable<<endl;
    cout<<" #Clauses  : "<<_clauses.size()<<endl;
    cout<<" #Literals : "<<getLitnum()<<endl;
    cout<<"===========================\n";
};

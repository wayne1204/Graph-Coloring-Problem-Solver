/****************************************************************************
  FileName     [ sat.h ]
  PackageName  [ sat ]
  Synopsis     [ Define miniSat solver interface functions ]
  Author       [ Chung-Yang (Ric) Huang, Cheng-Yin Wu ]
  Copyright    [ Copyleft(c) 2010-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef SAT_H
#define SAT_H

#include <cassert>
#include <iostream>
#include "Solver.h"
#include <cmath>

using namespace std;

/********** MiniSAT_Solver **********/
class SatSolver
{
   public : 
      SatSolver():_solver(0) { }
      ~SatSolver() { if (_solver) delete _solver; }

      // Solver initialization and reset
      void initialize() {
         reset();
         if (_curVar == 0) { _solver->newVar(); ++_curVar; }
      }
      void reset() {
         if (_solver) delete _solver;
         _solver = new Solver();
         _assump.clear(); _curVar = 0;
      }

      // Constructing proof model
      // Return the Var ID of the new Var
      inline Var newVar() { _solver->newVar(); return _curVar++; }
      // fa/fb = true if it is inverted
      void addlitCNF(Var v, bool b){             //(v)
         vec<Lit> lits;
         Lit l = b? Lit(v): ~Lit(v);
         lits.push(l);
         _solver->addClause(lits);
      }

      void addArrCNF(Var* v, int& count){           // (va + vb + vc ......)
         vec<Lit> lits;
         for(size_t i=0; i<count; ++i){
            Lit el = Lit( *(v+i) );
            lits.push(el);
         }
         _solver->addClause(lits); lits.clear();
      }
      void addNodeCNF(Var* v, int& diff_num, int& log_num){
         vec<Lit> lits;
         for(int i=0;i<diff_num;i++){
            for(int j=0;j<log_num; j++){
                int divider = pow(2,j);
               Lit ele = Lit( *(v+j) );
               if ( (i/divider ) %2 ==0)    lits.push(ele);
               else                        lits.push(~ele);
            }
            _solver->addClause(lits); lits.clear();
         }
      }
      void addAdjCNF(Var* vec1, Var* vec2, int& log_color, int& diff_num){
         vec<Lit> lits;
         for(int i=diff_num; i< pow(2,log_color); i++){
            for(int j=0;j<log_color; j++){
               int divider = pow(2,j);
               Lit ele1 = Lit( *(vec1+j) );
               Lit ele2 = Lit( *(vec2+j) );
               if ( (i/divider ) %2 ==0){
                  lits.push(ele1);          lits.push(ele2);
               }    
               else {
                  lits.push(~ele1);         lits.push(~ele2);
               }
            }
            _solver->addClause(lits); lits.clear();       
         }
      }
      void AdjXorCNF(Var vf, Var va, Var vb) {
         vec<Lit> lits;
         Lit lf = Lit(vf); Lit la = Lit(va); Lit lb = Lit(vb);
         lits.push(~la); lits.push( lb); lits.push( lf);
         _solver->addClause(lits); lits.clear();
         lits.push( la); lits.push(~lb); lits.push( lf);
         _solver->addClause(lits); lits.clear();
         lits.push( la); lits.push( lb); lits.push(~lf);
         _solver->addClause(lits); lits.clear();
         lits.push(~la); lits.push(~lb); lits.push(~lf);
         _solver->addClause(lits); lits.clear();
      }
      void addNegCNF(Var va, Var vb){   // (va' + vb')
         vec<Lit> lits;
         Lit la =  ~Lit(va);     Lit lb =  ~Lit(vb);
         lits.push(la); lits.push(lb);
         _solver->addClause(lits); 
      }
      void addCmdCNF(Var vf, Var va, Var vb) {      // (va + vb -> vf)
         vec<Lit> lits;
         Lit lf = Lit(vf);  Lit la = Lit(va);    Lit lb = Lit(vb);
         lits.push(~la); lits.push(lf);
         _solver->addClause(lits); lits.clear();
         lits.push(~lb); lits.push(lf);
         _solver->addClause(lits); lits.clear();
         lits.push(la); lits.push(lb); lits.push(~lf);
         _solver->addClause(lits); lits.clear();
      }
      void addCmdCNF(Var vf, Var va, Var vb, Var vc) {  // (va + vb + vc -> vf)
         vec<Lit> lits;
         Lit lf = Lit(vf);
         Lit la = Lit(va);  Lit lb = Lit(vb);     Lit lc = Lit(vc);
         lits.push(~la); lits.push(lf);
         _solver->addClause(lits); lits.clear();
         lits.push(~lb); lits.push(lf);
         _solver->addClause(lits); lits.clear();
         lits.push(~lc); lits.push(lf);
         _solver->addClause(lits); lits.clear();
         lits.push(la); lits.push(lb); lits.push(lc); lits.push(~lf);
         _solver->addClause(lits); lits.clear();
      }
      void addEqlCNF(Var va, Var vb){
         vec<Lit> lits;
         Lit la = Lit(va);       Lit lb = Lit(vb);
         lits.push(~la); lits.push(lb);
         _solver->addClause(lits); lits.clear();
         lits.push(la); lits.push(~lb);
         _solver->addClause(lits); lits.clear();
      }
      void addAigCNF(Var vf, Var va, bool fa, Var vb, bool fb) {
         vec<Lit> lits;
         Lit lf = Lit(vf);
         Lit la = fa? ~Lit(va): Lit(va);
         Lit lb = fb? ~Lit(vb): Lit(vb);
         lits.push(la); lits.push(~lf);
         _solver->addClause(lits); lits.clear();
         lits.push(lb); lits.push(~lf);
         _solver->addClause(lits); lits.clear();
         lits.push(~la); lits.push(~lb); lits.push(lf);
         _solver->addClause(lits); lits.clear();
      }
      // fa/fb = true if it is inverted
      void addXorCNF(Var vf, Var va, bool fa, Var vb, bool fb) {
         vec<Lit> lits;
         Lit lf = Lit(vf);
         Lit la = fa? ~Lit(va): Lit(va);
         Lit lb = fb? ~Lit(vb): Lit(vb);
         lits.push(~la); lits.push( lb); lits.push( lf);
         _solver->addClause(lits); lits.clear();
         lits.push( la); lits.push(~lb); lits.push( lf);
         _solver->addClause(lits); lits.clear();
         lits.push( la); lits.push( lb); lits.push(~lf);
         _solver->addClause(lits); lits.clear();
         lits.push(~la); lits.push(~lb); lits.push(~lf);
         _solver->addClause(lits); lits.clear();
      }

      // For incremental proof, use "assumeSolve()"
      void assumeRelease() { _assump.clear(); }
      void assumeProperty(Var prop, bool val) {
         _assump.push(val? Lit(prop): ~Lit(prop));
      }
      bool assumpSolve() { return _solver->solve(_assump); }

      // For one time proof, use "solve"
      void assertProperty(Var prop, bool val) {
         _solver->addUnit(val? Lit(prop): ~Lit(prop));
      }
      bool solve() { _solver->solve(); return _solver->okay(); }

      // Functions about Reporting
      // Return 1/0/-1; -1 means unknown value
      int getValue(Var v) const {
         return (_solver->modelValue(v)==l_True?1:
                (_solver->modelValue(v)==l_False?0:-1)); }
      void printStats() const { const_cast<Solver*>(_solver)->printStats(); }

   private : 
      Solver           *_solver;    // Pointer to a Minisat solver
      Var               _curVar;    // Variable currently
      vec<Lit>          _assump;    // Assumption List for assumption solve
};

#endif  // SAT_H

